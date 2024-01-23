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

/**
 * brief Fonction permettant d'initialiser la boite aux lettres
 * 
 */
uint8_t Rfid_new(void);

/**
 * brief Fonction permettant de lancer le thread RFID
 */
uint8_t Rfid_start(void);

/**
 * brief Fonction permettant de stopper la lecture des badges RFID
 * 
 */
void Rfid_stop();

/**
 * brief Fonction permettant de libérer la mémoire et la boite aux lettres
 * 
 */
uint8_t Rfid_free(void);