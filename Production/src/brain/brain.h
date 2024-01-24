#ifndef BRAIN_H
#define BRAIN_H

/**
 * @brief Démarrage de l'application Visiolock
 * 
 */
void Brain_startVisiolock();

/**
 * @brief Arrêt de l'application Visiolock
 * 
 */
void Brain_stopVisiolock();

/**
 * @brief Mise en veille de l'application Visiolock
 * 
 */
void Brain_standBy();

/**
 * @brief Fonction appelée lorsqu'un tag RFID a été lu
 * 
 * @param idTag la valeur du tag RFID lu
 */
void Brain_tagReaded(char* idTag);

/**
 * @brief Fonction appelée lorsque l'on souhaite changer de mode
 * 
 * @param mode le nouveau mode
 */
void Brain_changeMode(Mode mode);

/**
 * @brief Fonction appelée lorsqu'un visage a été analysé
 * 
 * @param recognized la valeur de retour de la reconnaissance faciale
 */
void Brain_faceAnalysed(bool recognized);

void Brain_getCurrentTag(char* buffer, size_t size);

/**
 * @brief Fonction d'initialisation de l'objet Brain
 * 
 */
int Brain_new();

/**
 * @brief Fonction de démarrage de l'objet Brain
 * 
 */
int Brain_start();

/**
 * @brief Fonction de destruction de l'objet Brain
 * 
 */
int Brain_free();

#endif //BRAIN_H