#include <unistd.h>
#include "mfrc.h"
#include "read.h"
#include <gpiod.h>

#define RSTPIN  22
#define LOW     0
#define HIGH    1


void PCD_Reset() {
    PCD_WriteRegister(CommandReg, PCD_SoftReset);  // Émettre la commande SoftReset.
    
    // Le datasheet ne mentionne pas la durée nécessaire à l'achèvement de la commande SoftReset.
    // Mais le MFRC522 pourrait être en mode d'arrêt de puissance douce (déclenché par le bit 4 de CommandReg)
    // La section 8.8.2 du datasheet indique que le temps de démarrage de l'oscillateur est le temps de démarrage du cristal + 37,74 µs.
    // Soyons généreux : 50 ms.
    usleep(50);
    
    // Attendre que le bit PowerDown dans CommandReg soit effacé
    while (PCD_ReadRegister(CommandReg) & (1 << 4)) {
        // Le PCD redémarre toujours - peu probable après avoir attendu 50 ms, mais mieux vaut être prudent.
    }
} // Fin de PCD_Reset()


void PCD_Init() {
        // Lors de la communication avec un PICC, nous avons besoin d'une temporisation en cas de problème.
    // f_timer = 13,56 MHz / (2*TPreScaler+1) où TPreScaler = [TPrescaler_Hi:TPrescaler_Lo].
    // TPrescaler_Hi sont les quatre bits de poids faible dans TModeReg. TPrescaler_Lo est TPrescalerReg.
    PCD_WriteRegister(TModeReg, 0x80);           // TAuto=1; le minuteur démarre automatiquement à la fin de la transmission dans tous les modes de communication à toutes les vitesses
    PCD_WriteRegister(TPrescalerReg, 0xA9);      // TPreScaler = TModeReg[3..0]:TPrescalerReg, c'est-à-dire 0x0A9 = 169 => f_timer=40kHz, soit une période de minuterie de 25 µs.
    PCD_WriteRegister(TReloadRegH, 0x03);        // Recharger le minuteur avec 0x3E8 = 1000, soit 25 ms avant l'expiration du délai.
    PCD_WriteRegister(TReloadRegL, 0xE8);

    PCD_WriteRegister(TxASKReg, 0x40);          // Valeur par défaut 0x00. Forcer une modulation ASK à 100 % indépendamment du réglage du registre ModGsPReg
    PCD_WriteRegister(ModeReg, 0x3D);            // Valeur par défaut 0x3F. Définir la valeur prédéfinie pour le coprocesseur CRC pour la commande CalcCRC à 0x6363 (ISO 14443-3 partie 6.2.4)
    PCD_AntennaOn();                            // Activer les broches du pilote d'antenne TX1 et TX2 (elles ont été désactivées par la réinitialisation)
} // Fin de PCD_Init()


void PCD_AntennaOn() {
    byte value = PCD_ReadRegister(TxControlReg);
    if ((value & 0x03) != 0x03) {
        PCD_WriteRegister(TxControlReg, value | 0x03);
    }
} // Fin de PCD_AntennaOn()


bool PICC_IsNewCardPresent() {
    byte bufferATQA[2];
    memset(bufferATQA, 0, sizeof(bufferATQA));
    byte bufferSize = sizeof(bufferATQA);
    byte result = PICC_RequestA(bufferATQA, &bufferSize);
    return (result == STATUS_OK || result == STATUS_COLLISION);
} // Fin de PICC_IsNewCardPresent()

byte PICC_RequestA(byte *bufferATQA, byte *bufferSize) {
    return PICC_REQA_or_WUPA(PICC_CMD_REQA, bufferATQA, bufferSize);
} // Fin de PICC_RequestA()


byte PICC_REQA_or_WUPA(byte command, byte *bufferATQA, byte *bufferSize) {
    byte validBits;
    byte status;

    if (bufferATQA == NULL || *bufferSize < 2) {    // La réponse ATQA fait 2 octets.
        return STATUS_NO_ROOM;
    }
    PCD_ClearRegisterBitMask(CollReg, 0x80);        // ValuesAfterColl=1 => Les bits reçus après une collision sont effacés.
    validBits = 7;                                  // Pour REQA et WUPA, nous avons besoin du format de trame court - transmettez uniquement 7 bits du dernier (et unique) octet. TxLastBits = BitFramingReg[2..0]
    status = PCD_TransceiveData(&command, 1, bufferATQA, bufferSize, &validBits, 0, false);
    if (status != STATUS_OK) {
        return status;
    }
    if (*bufferSize != 2 || validBits != 0) {      // ATQA doit être exactement de 16 bits.
        return STATUS_ERROR;
    }
    return STATUS_OK;
} // Fin de PICC_REQA_or_WUPA()

void PCD_ClearRegisterBitMask(byte reg, byte mask) {
    byte tmp;
    tmp = PCD_ReadRegister(reg);
    PCD_WriteRegister(reg, tmp & (~mask));    // Efface le masque de bits
} // Fin de PCD_ClearRegisterBitMask()


byte PCD_ReadRegister(byte reg) {
   /*char data[2];
    data[0] = 0x80 | ((reg) & 0x7E);                // !!!!!!!!!!! Ancienne version !!!!!!!!!!
    bcm2835_spi_transfern(data, 2);*/

    uint8_t tx_data[2] = {0x80 | ((reg) & 0x7E), 0};
    uint8_t rx_data[10];
    memset(rx_data, 0, sizeof(rx_data));
    if (spi_transfern(tx_data, rx_data, 2) == -1) {
        return 0;
    }
    return (byte)rx_data[1];
} // Fin de PCD_ReadRegister()

void PCD_ReadRegisterb(byte reg, byte count, byte *values, byte rxAlign) {
    if (count == 0) {
        return;
    }

    // MSB == 1 is for reading. LSB is not used in the address. Datasheet section 8.1.2.3.
    byte address = 0x80 | (reg & 0x7E);
    byte index = 0;                         // Index in the values array.
    count--;                               // One read is performed outside of the loop

    // Send the first byte
    uint8_t tx_buffer[1] = { address };
    uint8_t rx_buffer[1];
    spi_transfer(tx_buffer, rx_buffer, 1);

    while (index < count) {
        if (index == 0 && rxAlign) {       // Only update bit positions rxAlign..7 in values[0]
            // Create a bitmask for bit positions rxAlign..7
            byte mask = 0;
            for (byte i = rxAlign; i <= 7; i++) {
                mask |= (1 << i);
            }
            // Read the value and indicate that we want to read the same address again.
            uint8_t value;
            tx_buffer[0] = address;
            spi_transfer(tx_buffer, &value, 1);
            // Apply the mask to both the current value of values[0] and the new data in value.
            values[0] = (values[index] & ~mask) | (value & mask);
        }
        else { // Normal case
            tx_buffer[0] = address;
            spi_transfer(tx_buffer, &values[index], 1);
        }
        index++;
    }

    // Read the final byte. Send 0 to stop reading.
    tx_buffer[0] = 0;
    spi_transfer(tx_buffer, &values[index], 1);
}

void PCD_WriteRegister(byte reg, byte value) {
    uint8_t tx_data[2];
    tx_data[0] = reg & 0x7E;
    tx_data[1] = value;
    uint8_t rx_data[2];
    memset(rx_data, 0, sizeof(rx_data));
    if (spi_transfern(tx_data, rx_data, 2) == -1) {
        printf("Erreur de transfer");
    }
} // Fin de PCD_WriteRegister()


void PCD_WriteRegisterb(byte reg, byte count, byte *values) {
    for (byte index = 0; index < count; index++) {
        PCD_WriteRegister(reg, values[index]);
    }
} // Fin de PCD_WriteRegister()

byte PCD_TransceiveData(byte *sendData, byte sendLen, byte *backData, byte *backLen, byte *validBits, byte rxAlign, bool checkCRC) {
    byte waitIRq = 0x30; // RxIRq and IdleIRq
    return PCD_CommunicateWithPICC(PCD_Transceive, waitIRq, sendData, sendLen, backData, backLen, validBits, rxAlign, checkCRC);
} // Fin de PCD_TransceiveData()


byte PCD_CommunicateWithPICC(byte command, byte waitIRq, byte *sendData, byte sendLen, byte *backData, byte *backLen, byte *validBits, byte rxAlign, bool checkCRC) {
    byte n, _validBits;
    unsigned int i;

    // Préparer les valeurs pour BitFramingReg
    byte txLastBits = validBits ? *validBits : 0;
    byte bitFraming = (rxAlign << 4) + txLastBits; // RxAlign = BitFramingReg[6..4]. TxLastBits = BitFramingReg[2..0]

    PCD_WriteRegister(CommandReg, PCD_Idle);       // Arrêter toute commande active.
    PCD_WriteRegister(ComIrqReg, 0x7F);             // Effacer les sept bits de demande d'interruption
    PCD_SetRegisterBitMask(FIFOLevelReg, 0x80);     // FlushBuffer = 1, initialisation du FIFO
    PCD_WriteRegisterb(FIFODataReg, sendLen, sendData); // Écrire sendData dans le FIFO
    PCD_WriteRegister(BitFramingReg, bitFraming);   // Ajustements de bits
    PCD_WriteRegister(CommandReg, command);          // Exécuter la commande
    if (command == PCD_Transceive) {
        PCD_SetRegisterBitMask(BitFramingReg, 0x80); // StartSend=1, la transmission de données commence
    }

    // Attendre que la commande soit terminée.
    // Dans PCD_Init(), nous activons le drapeau TAuto dans TModeReg. Cela signifie que le minuteur démarre automatiquement lorsque le PCD cesse de transmettre.
    // Chaque itération de la boucle do-while prend 17,86 µs.
    i = 2000;
    while (1) {
        n = PCD_ReadRegister(ComIrqReg); // Les bits ComIrqReg[7..0] sont : Set1 TxIRq RxIRq IdleIRq HiAlertIRq LoAlertIRq ErrIRq TimerIRq
        if (n & waitIRq) {                  // L'une des interruptions signalant la réussite a été activée.
            break;
        }
        if (n & 0x01) {                     // Minuterie expirée - rien reçu en 25 ms
            return STATUS_TIMEOUT;
        }
        if (--i == 0) {                     // L'arrêt d'urgence. Si toutes les autres conditions échouent, nous terminerons finalement sur celle-ci après 35,7 ms. La communication avec le MFRC522 pourrait être interrompue.
            return STATUS_TIMEOUT;
        }
    }

    // S'arrêter maintenant si des erreurs, sauf les collisions, ont été détectées.
    byte errorRegValue = PCD_ReadRegister(ErrorReg); // Les bits ErrorReg[7..0] sont : WrErr TempErr réservé BufferOvfl CollErr CRCErr ParityErr ProtocolErr
    if (errorRegValue & 0x13) {    // BufferOvfl ParityErr ProtocolErr
        return STATUS_ERROR;
    }

    // Si l'appelant veut des données en retour, les obtenir du MFRC522.
    if (backData && backLen) {
        n = PCD_ReadRegister(FIFOLevelReg);            // Nombre d'octets dans le FIFO
        if (n > *backLen) {
            return STATUS_NO_ROOM;
        }
        *backLen = n;                                       // Nombre d'octets retournés
        PCD_ReadRegisterb(FIFODataReg, n, backData, rxAlign); // Obtenir les données reçues du FIFO
        _validBits = PCD_ReadRegister(ControlReg) & 0x07;   // RxLastBits[2:0] indique le nombre de bits valides dans le dernier octet reçu. Si cette valeur est 000b, l'ensemble de l'octet est valide.
        if (validBits) {
            *validBits = _validBits;
        }
    }

    // Signaler les collisions
    if (errorRegValue & 0x08) {        // CollErr
        return STATUS_COLLISION;
    }

    // Effectuer la validation CRC_A si demandé.
    if (backData && backLen && checkCRC) {
        // Dans ce cas, un NAK MIFARE Classic n'est pas OK.
        if (*backLen == 1 && _validBits == 4) {
            return STATUS_MIFARE_NACK;
        }
        // Nous avons besoin d'au moins la valeur CRC_A et tous les 8 bits du dernier octet doivent être reçus.
        if (*backLen < 2 || _validBits != 0) {
            return STATUS_CRC_WRONG;
        }
        // Vérifier CRC_A - faire notre propre calcul et stocker le contrôle dans controlBuffer.
        byte controlBuffer[2];
        n = PCD_CalculateCRC(&backData[0], *backLen - 2, &controlBuffer[0]);
        if (n != STATUS_OK) {
            return n;
        }
        if ((backData[*backLen - 2] != controlBuffer[0]) || (backData[*backLen - 1] != controlBuffer[1])) {
            return STATUS_CRC_WRONG;
        }
    }

    return STATUS_OK;
} // Fin de PCD_CommunicateWithPICC()

void PCD_SetRegisterBitMask(byte reg, byte mask) {
    byte tmp;
    tmp = PCD_ReadRegister(reg);
    PCD_WriteRegister(reg, tmp | mask); // Définir le masque de bits
} // Fin de PCD_SetRegisterBitMask()

byte PCD_CalculateCRC(byte *data, byte length, byte *result) {
    PCD_WriteRegister(CommandReg, PCD_Idle);      // Stop any active command.
    PCD_WriteRegister(DivIrqReg, 0x04);            // Clear the CRCIRq interrupt request bit
    PCD_SetRegisterBitMask(FIFOLevelReg, 0x80);    // FlushBuffer = 1, FIFO initialization
    PCD_WriteRegisterb(FIFODataReg, length, data);  // Write data to the FIFO
    PCD_WriteRegister(CommandReg, PCD_CalcCRC);    // Start the calculation

    // Wait for the CRC calculation to complete. Each iteration of the while-loop takes 17.73µs.
    word i = 5000;
    byte n;
    while (1) {
        n = PCD_ReadRegister(DivIrqReg);  // DivIrqReg[7..0] bits are: Set2 reserved reserved MfinActIRq reserved CRCIRq reserved reserved
        if (n & 0x04) {                    // CRCIRq bit set - calculation done
            break;
        }
        if (--i == 0) {                    // The emergency break. We will eventually terminate on this one after 89ms. Communication with the MFRC522 might be down.
            return STATUS_TIMEOUT;
        }
    }
    PCD_WriteRegister(CommandReg, PCD_Idle);      // Stop calculating CRC for new content in the FIFO.

    // Transfer the result from the registers to the result buffer
    result[0] = PCD_ReadRegister(CRCResultRegL);
    result[1] = PCD_ReadRegister(CRCResultRegH);
    return STATUS_OK;
} // End PCD_CalculateCRC()


bool PICC_ReadCardSerial(MFRC522 *mfr) {
    byte result = PICC_Selected(&mfr->Uid);
    //printf("le resultat de PICC_select %d", result);
    return (result == STATUS_OK);
} // End PICC_ReadCardSerial()

byte PICC_Selected(Uid *uid) {
    bool uidComplete;
    bool selectDone;
    bool useCascadeTag;
    byte cascadeLevel = 1;
    byte result;
    byte count;
    byte index;
    byte uidIndex;
    signed char currentLevelKnownBits;
    byte buffer[9];
    memset(buffer, 0, sizeof(buffer));
    byte bufferUsed;
    byte rxAlign;
    byte txLastBits;
    byte *responseBuffer;
    byte responseLength;

    byte validBits = 7;

    if (validBits > 80) {
        return STATUS_INVALID;
    }

    PCD_ClearRegisterBitMask(CollReg, 0x80);

    uidComplete = false;
    while (!uidComplete) {
        switch (cascadeLevel) {
        case 1:
            buffer[0] = PICC_CMD_SEL_CL1;
            uidIndex = 0;
            useCascadeTag = validBits && uid->size > 4;
            printf("uid sieze > 4");
            break;

        case 2:
            buffer[0] = PICC_CMD_SEL_CL2;
            uidIndex = 3;
            useCascadeTag = validBits && uid->size > 7;
            printf("uid sieze > 7");
            break;

        case 3:
            buffer[0] = PICC_CMD_SEL_CL3;
            uidIndex = 6;
            printf("uid sieze ????");
            useCascadeTag = false;
            break;

        default:
            return STATUS_INTERNAL_ERROR;
            break;
        }

        currentLevelKnownBits = validBits - (8 * uidIndex);
        if (currentLevelKnownBits < 0) {
            currentLevelKnownBits = 0;
        }

        index = 2;
        if (useCascadeTag) {
            buffer[index++] = PICC_CMD_CT;
        }
        byte bytesToCopy = 6;//currentLevelKnownBits / 8 + (currentLevelKnownBits % 8 ? 1 : 0);
        if (bytesToCopy) {
            byte maxBytes = useCascadeTag ? 3 : 4;
            if (bytesToCopy > maxBytes) {
                bytesToCopy = maxBytes;
            }
           
            for (count = 0; count < bytesToCopy; count++) {
                buffer[index++] = uid->uidByte[uidIndex + count];
                //uid->uidByte[uidIndex + count] = buffer[index++] ;
                printf("uid uidByte %x", uid->uidByte[count]); 
            }
            printf("\n");
        }

        if (useCascadeTag) {
            currentLevelKnownBits += 8;
        }

        selectDone = false;
        while (!selectDone) {
            if (currentLevelKnownBits >= 32) {
                buffer[1] = 0x70;
                buffer[6] = buffer[2] ^ buffer[3] ^ buffer[4] ^ buffer[5];
                result = PCD_CalculateCRC(buffer, 7, &buffer[7]);
                if (result != STATUS_OK) {
                    return result;
                }
                txLastBits = 0;
                bufferUsed = 9;
                responseBuffer = &buffer[6];
                responseLength = 3;
            }
            else {
                txLastBits = currentLevelKnownBits % 8;
                count = currentLevelKnownBits / 8;
                index = 2 + count;
                buffer[1] = (index << 4) + txLastBits;
                bufferUsed = index + (txLastBits ? 1 : 0);
                responseBuffer = &buffer[index];
                responseLength = sizeof(buffer) - index;
            }

            rxAlign = txLastBits;
            PCD_WriteRegister(BitFramingReg, (rxAlign << 4) + txLastBits);

            result = PCD_TransceiveData(buffer, bufferUsed, responseBuffer, &responseLength, &txLastBits, rxAlign, false);
            if (result == STATUS_COLLISION) {
                result = PCD_ReadRegister(CollReg);
                if (result & 0x20) {
                    return STATUS_COLLISION;
                }
                byte collisionPos = result & 0x1F;
                if (collisionPos == 0) {
                    collisionPos = 32;
                }
                if (collisionPos <= currentLevelKnownBits) {
                    return STATUS_INTERNAL_ERROR;
                }
                currentLevelKnownBits = collisionPos;
                count = (currentLevelKnownBits - 1) % 8;
                index = 1 + (currentLevelKnownBits / 8) + (count ? 1 : 0);
                buffer[index] |= (1 << count);
            }
            else if (result != STATUS_OK) {
                return result;
            }
            else {
                if (currentLevelKnownBits >= 32) {
                    selectDone = true;
                }
                else {
                    currentLevelKnownBits = 32;
                }
            }
        }

        if (responseLength != 3 || txLastBits != 0) {
            return STATUS_ERROR;
        }

        result = PCD_CalculateCRC(responseBuffer, 1, &buffer[2]);
        if (result != STATUS_OK) {
            return result;
        }
        if ((buffer[2] != responseBuffer[1]) || (buffer[3] != responseBuffer[2])) {
            return STATUS_CRC_WRONG;
        }
        if (responseBuffer[0] & 0x04) {
            cascadeLevel++;
        }
        else {
            uidComplete = true;
            uid->sak = responseBuffer[0];
        }
    }

    uid->size = 3 * cascadeLevel + 1;

    return STATUS_OK;
} // End PICC_Select()

