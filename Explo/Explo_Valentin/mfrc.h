/**
 * MFRC522.h - Bibliothèque pour utiliser le MODULE RFID ARDUINO KIT 13.56 MHZ AVEC ÉTIQUETTES SPI W ET R PAR COOQROBOT.
 * Basé sur le code de Dr.Leong (WWW.B2CQSHOP.COM)
 * Créé par Miguel Balboa (circuitito.com), janv. 2012.
 * Réécrit par Søren Thing Andersen (access.thing.dk), automne 2013 (Traduction en anglais, refactoré, commentaires, anti-collision, niveaux de cascade.)
 * Étendu par Tom Clement avec la fonctionnalité d'écrire sur le secteur 0 des cartes Mifare à UID modifiable.
 * Libéré dans le domaine public.
 * 
-- Repensé pour s'adapter à Raspberry Pi ---

*/
#ifndef MFRC522_h
#define MFRC522_h

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


typedef uint8_t byte;
typedef uint16_t word;

typedef uint8_t byte;
typedef uint16_t word;

void PCD_Reset();
void PCD_WriteRegisterb(byte reg, byte count, byte *values);
void PCD_ReadRegisterb(byte reg, byte count, byte *values, byte rxAlign);

/*
const byte MFRC522_firmware_referenceV1_0[]  = {
    0x00, 0xC6, 0x37, 0xD5, 0x32, 0xB7, 0x57, 0x5C,
    0xC2, 0xD8, 0x7C, 0x4D, 0xD9, 0x70, 0xC7, 0x73,
    0x10, 0xE6, 0xD2, 0xAA, 0x5E, 0xA1, 0x3E, 0x5A,
    0x14, 0xAF, 0x30, 0x61, 0xC9, 0x70, 0xDB, 0x2E,
    0x64, 0x22, 0x72, 0xB5, 0xBD, 0x65, 0xF4, 0xEC,
    0x22, 0xBC, 0xD3, 0x72, 0x35, 0xCD, 0xAA, 0x41,
    0x1F, 0xA7, 0xF3, 0x53, 0x14, 0xDE, 0x7E, 0x02,
    0xD9, 0x0F, 0xB5, 0x5E, 0x25, 0x1D, 0x29, 0x79
};

// Version 2.0
const byte MFRC522_firmware_referenceV2_0[] = {
    0x00, 0xEB, 0x66, 0xBA, 0x57, 0xBF, 0x23, 0x95,
    0xD0, 0xE3, 0x0D, 0x3D, 0x27, 0x89, 0x5C, 0xDE,
    0x9D, 0x3B, 0xA7, 0x00, 0x21, 0x5B, 0x89, 0x82,
    0x51, 0x3A, 0xEB, 0x02, 0x0C, 0xA5, 0x00, 0x49,
    0x7C, 0x84, 0x4D, 0xB3, 0xCC, 0xD2, 0x1B, 0x81,
    0x5D, 0x48, 0x76, 0xD5, 0x71, 0x61, 0x21, 0xA9,
    0x86, 0x96, 0x83, 0x38, 0xCF, 0x9D, 0x5B, 0x6D,
    0xDC, 0x15, 0xBA, 0x3E, 0x7D, 0x95, 0x3B, 0x2F
};*/



typedef struct {
		byte size;
		byte uidByte[10];
		byte sak;
	} Uid;

typedef struct {
		byte keyByte[6];
	} MIFARE_Key;

typedef struct{
    /* data */


	// Définition des registres du module RFID MFRC522
enum PCD_Register {
    CommandReg          = 0x01 << 1,
    ComIEnReg           = 0x02 << 1,
    DivIEnReg           = 0x03 << 1,
    ComIrqReg           = 0x04 << 1,
    DivIrqReg           = 0x05 << 1,
    ErrorReg            = 0x06 << 1,
    Status1Reg          = 0x07 << 1,
    Status2Reg          = 0x08 << 1,
    FIFODataReg         = 0x09 << 1,
    FIFOLevelReg        = 0x0A << 1,
    WaterLevelReg       = 0x0B << 1,
    ControlReg          = 0x0C << 1,
    BitFramingReg       = 0x0D << 1,
    CollReg             = 0x0E << 1,
    ModeReg             = 0x11 << 1,
    TxModeReg           = 0x12 << 1,
    RxModeReg           = 0x13 << 1,
    TxControlReg        = 0x14 << 1,
    TxASKReg            = 0x15 << 1,
    TxSelReg            = 0x16 << 1,
    RxSelReg            = 0x17 << 1,
    RxThresholdReg      = 0x18 << 1,
    DemodReg            = 0x19 << 1,
    MfTxReg             = 0x1C << 1,
    MfRxReg             = 0x1D << 1,
    SerialSpeedReg      = 0x1F << 1,
    CRCResultRegH       = 0x21 << 1,
    CRCResultRegL       = 0x22 << 1,
    ModWidthReg         = 0x24 << 1,
    RFCfgReg            = 0x26 << 1,
    GsNReg              = 0x27 << 1,
    CWGsPReg            = 0x28 << 1,
    ModGsPReg           = 0x29 << 1,
    TModeReg            = 0x2A << 1,
    TPrescalerReg       = 0x2B << 1,
    TReloadRegH         = 0x2C << 1,
    TReloadRegL         = 0x2D << 1,
    TCounterValueRegH   = 0x2E << 1,
    TCounterValueRegL   = 0x2F << 1,
    TestSel1Reg         = 0x31 << 1,
    TestSel2Reg         = 0x32 << 1,
    TestPinEnReg        = 0x33 << 1,
    TestPinValueReg     = 0x34 << 1,
    TestBusReg          = 0x35 << 1,
    AutoTestReg         = 0x36 << 1,
    VersionReg          = 0x37 << 1,
    AnalogTestReg       = 0x38 << 1,
    TestDAC1Reg         = 0x39 << 1,
    TestDAC2Reg         = 0x3A << 1,
    TestADCReg          = 0x3B << 1
};

// Commandes MFRC522
enum PCD_Command {
    PCD_Idle                = 0x00,
    PCD_Mem                 = 0x01,
    PCD_GenerateRandomID    = 0x02,
    PCD_CalcCRC             = 0x03,
    PCD_Transmit            = 0x04,
    PCD_NoCmdChange         = 0x07,
    PCD_Receive             = 0x08,
    PCD_Transceive          = 0x0C,
    PCD_MFAuthent           = 0x0E,
    PCD_SoftReset           = 0x0F
};

// Masques RxGain[2:0], définit le facteur de gain du signal du récepteur (sur le PCD).
enum PCD_RxGain {
    RxGain_18dB             = 0x00 << 4,
    RxGain_23dB             = 0x01 << 4,
    RxGain_18dB_2           = 0x02 << 4,
    RxGain_23dB_2           = 0x03 << 4,
    RxGain_33dB             = 0x04 << 4,
    RxGain_38dB             = 0x05 << 4,
    RxGain_43dB             = 0x06 << 4,
    RxGain_48dB             = 0x07 << 4,
    RxGain_min              = 0x00 << 4,
    RxGain_avg              = 0x04 << 4,
    RxGain_max              = 0x07 << 4
};

// Commandes PICC
enum PICC_Command {
    PICC_CMD_REQA            = 0x26,
    PICC_CMD_WUPA            = 0x52,
    PICC_CMD_CT              = 0x88,
    PICC_CMD_SEL_CL1         = 0x93,
    PICC_CMD_SEL_CL2         = 0x95,
    PICC_CMD_SEL_CL3         = 0x97,
    PICC_CMD_HLTA            = 0x50,
    PICC_CMD_MF_AUTH_KEY_A   = 0x60,
    PICC_CMD_MF_AUTH_KEY_B   = 0x61,
    PICC_CMD_MF_READ         = 0x30,
    PICC_CMD_MF_WRITE        = 0xA0,
    PICC_CMD_MF_DECREMENT    = 0xC0,
    PICC_CMD_MF_INCREMENT    = 0xC1,
    PICC_CMD_MF_RESTORE      = 0xC2,
    PICC_CMD_MF_TRANSFER     = 0xB0,
    PICC_CMD_UL_WRITE        = 0xA2
};

// Constantes MIFARE
enum MIFARE_Misc {
    MF_ACK          = 0xA,
    MF_KEY_SIZE     = 6
};

// Types de PICC
enum PICC_Type {
    PICC_TYPE_UNKNOWN       = 0,
    PICC_TYPE_ISO_14443_4    = 1,
    PICC_TYPE_ISO_18092      = 2,
    PICC_TYPE_MIFARE_MINI    = 3,
    PICC_TYPE_MIFARE_1K      = 4,
    PICC_TYPE_MIFARE_4K      = 5,
    PICC_TYPE_MIFARE_UL      = 6,
    PICC_TYPE_MIFARE_PLUS    = 7,
    PICC_TYPE_TNP3XXX        = 8,
    PICC_TYPE_NOT_COMPLETE   = 255
};

// Codes de retour des fonctions
enum StatusCode {
    STATUS_OK               = 1,
    STATUS_ERROR            = 2,
    STATUS_COLLISION        = 3,
    STATUS_TIMEOUT          = 4,
    STATUS_NO_ROOM          = 5,
    STATUS_INTERNAL_ERROR   = 6,
    STATUS_INVALID          = 7,
    STATUS_CRC_WRONG        = 8,
    STATUS_MIFARE_NACK      = 9
};

	
	Uid Uid;
	
    MIFARE_Key MIFARE_Key;

	
	
	
	
}MFRC522;

static const byte FIFO_SIZE = 64;
	
//void initSPI();
void setSPIConfig();

void PCD_WriteRegister(byte reg, byte value);
//void PCD_WriteRegisterb(byte reg, byte count, byte *values);
byte PCD_ReadRegister(byte reg);
//void PCD_ReadRegisterb(byte reg, byte count, byte *values, byte rxAlign);
void setBitMask(unsigned char reg, unsigned char mask);
void PCD_SetRegisterBitMask(byte reg, byte mask);
void PCD_ClearRegisterBitMask(byte reg, byte mask);
byte PCD_CalculateCRC(byte *data, byte length, byte *result);

void PCD_Init();
//void PCD_Reset();
void PCD_AntennaOn();
void PCD_AntennaOff();
byte PCD_GetAntennaGain();
void PCD_SetAntennaGain(byte mask);
bool PCD_PerformSelfTest();

byte PCD_TransceiveData(byte *sendData, byte sendLen, byte *backData, byte *backLen, byte *validBits, byte rxAlign, bool checkCRC);
byte PCD_CommunicateWithPICC(byte command, byte waitIRq, byte *sendData, byte sendLen, byte *backData, byte *backLen, byte *validBits, byte rxAlign, bool checkCRC);
byte PICC_RequestA(byte *bufferATQA, byte *bufferSize);
byte PICC_WakeupA(byte *bufferATQA, byte *bufferSize);
byte PICC_REQA_or_WUPA(byte command, byte *bufferATQA, byte *bufferSize);
byte PICC_Select(Uid *uid, byte validBits);
byte PICC_HaltA();

byte PCD_Authenticate(byte command, byte blockAddr, MIFARE_Key *key, Uid *uid);
void PCD_StopCrypto1();
byte MIFARE_Read(byte blockAddr, byte *buffer, byte *bufferSize);
byte MIFARE_Write(byte blockAddr, byte *buffer, byte bufferSize);
byte MIFARE_Decrement(byte blockAddr, long delta);
byte MIFARE_Increment(byte blockAddr, long delta);
byte MIFARE_Restore(byte blockAddr);
byte MIFARE_Transfer(byte blockAddr);
byte MIFARE_Ultralight_Write(byte page, byte *buffer, byte bufferSize);
byte MIFARE_GetValue(byte blockAddr, long *value);
byte MIFARE_SetValue(byte blockAddr, long value);

byte PCD_MIFARE_Transceive(byte *sendData, byte sendLen, bool acceptTimeout);
const char *GetStatusCodeName(byte code);
byte PICC_GetType(byte sak);
const char *PICC_GetTypeName(byte type);
void PICC_DumpToSerial(Uid *uid);
void PICC_DumpMifareClassicToSerial(Uid *uid, byte piccType, MIFARE_Key *key);
void PICC_DumpMifareClassicSectorToSerial(Uid *uid, MIFARE_Key *key, byte sector);
void PICC_DumpMifareUltralightToSerial();
void MIFARE_SetAccessBits(byte *accessBitBuffer, byte g0, byte g1, byte g2, byte g3);
bool MIFARE_OpenUidBackdoor(bool logErrors);
bool MIFARE_SetUid(byte *newUid, byte uidSize, bool logErrors);
bool MIFARE_UnbrickUidSector(bool logErrors);

bool PICC_IsNewCardPresent();
bool PICC_ReadCardSerial();





#endif
