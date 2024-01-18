#ifndef _MFRC522_WRAPPER_H
#define _MFRC522_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MFRC522 MFRC522;

MFRC522 * newMRFC522();

void MFRC522_fct_PCD_Init(MFRC522** MFRC522);

bool MRFC522_fct_PICC_IsNewCardPresent(MFRC522** MFRC522);

bool MRFC522_fct_PICC_ReadCardSerial(MFRC522** MFRC522);

int MFRC522_uid_size(MFRC522** MFRC522);

int MFRC522_uid_uidByte(MFRC522** MFRC522, int number);

int MFRC522_uid_sak(MFRC522** MFRC522);


void deleteMFRC522(MFRC522** MFRC522);

#ifdef __cplusplus
}
#endif
#endif
