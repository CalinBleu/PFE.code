#ifndef GUARD_H
#define GUARD_H

#include "common.h"


/**
 * @brief Fonction qui vérifie le mot de passe
 * 
 * @param password le mot de passe
 * @param idTag l'idTag de l'utilisateur
 * 
 */
void Guard_checkPassword(char* password, char* idTag );


/**
 * @brief Fonction qui vérifie le tag RFID
 * 
 * @param idtag la valeur du tag RFID
 * @return AuthResult le résultat de la vérification
 */
AuthResult Guard_checkTag(char* idTag);


/**
 * @brief Fonction qui vérifie le résultat de la reconnaissance faciale
 * 
 * @param authResult le résultat de la reconnaissance faciale
 * 
 */
void Guard_resultRecognition(AuthResult authResult);


/**
 * @brief Fonction qui vérifie la tête de l'utilisateur
 * 
 * @param idtag la valeur du tag RFID lu
 * 
 */
void Guard_checkFace(char* idTag);

#endif //GUARD_H