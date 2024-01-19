#include <stdint.h>
/**
 * brief Fonction permettant de mettre en marche la lecture d'un badge RFID
 * 
 */
void Rfid_stopReading();

/**
 * brief Fonction permettant de lire un tag présenté au lecteur
 * 
 */
void Rfid_showTag();

/**
 * brief Fonction permettant de stopper la lecture après le passage d'un badge RFID
 * 
 */
void Rfid_startReading();

uint8_t Rfid_new(void);

uint8_t Rfid_start(void);

void Rfid_stop();

uint8_t Rfid_free(void);

