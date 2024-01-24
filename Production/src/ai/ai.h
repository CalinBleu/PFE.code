#ifndef AI_H
#define AI_H

#define CAMERA_IMAGES_PATH ("Camera/last_shot.jpg")

/**
 * brief Fonction permettant d'initialiser la boite aux lettres
 * 
 */
int AI_new(void);

/**
 * brief Fonction permettant de lancer le thread RFID
 */
int AI_start(void);

/**
 * brief Fonction permettant de libérer la mémoire et la boite aux lettres
 * 
 */
int AI_free(void);

/**
 * brief Fonction permettant de stopper la machine à états d'IA
 * 
 */
int AI_stop(void);

/**
 * brief Fonction d'envoi d'un message à la machine à états d'IA pour lancer la reconnaissance
 * 
 * param picturePath Chemin de l'image à analyser
 */
void AI_startRecognition(char *picturePath);

/**
 * brief Fonction d'envoi d'un message à la machine à états d'IA pour stopper la reconnaissance
 * 
 */
void AI_stopRecognition(void);


#endif //AI_H