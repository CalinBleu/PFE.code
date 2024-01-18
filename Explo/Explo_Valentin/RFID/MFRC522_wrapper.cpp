#include "MFRC522.h"
#include "MFRC522_wrapper.h"

extern "C" {
	MFRC522 * newMRFC522() {
		return new MFRC522;
	}

    void MFRC522_fct_PCD_Init(MFRC522** MFRC522){
        (*MFRC522)->PCD_Init();
    }

	bool MRFC522_fct_PICC_IsNewCardPresent(MFRC522** MFRC522) {
		return (*MFRC522)->PICC_IsNewCardPresent();
	}

    bool MRFC522_fct_PICC_ReadCardSerial(MFRC522** MFRC522) {
		return (*MFRC522)->PICC_ReadCardSerial();
	}

    int MFRC522_uid_size(MFRC522** MFRC522){
        return (*MFRC522)->uid.size;
    }

    int MFRC522_uid_uidByte(MFRC522** MFRC522, int number){
        return (*MFRC522)->uid.uidByte[number];
    }

    int MFRC522_uid_sak(MFRC522** MFRC522){
        return (*MFRC522)->uid.sak;
    }

/*
	void deleteMFRC522(MRFC522 * MFRC522) {
		delete MRFC522;
	}
    */
}
