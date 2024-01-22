#ifndef GUARD_H
#define GUARD_H

#include "common.h"

//fonction qui vérifie le mot de passe
void Guard_checkPassword(char* password, char* idTag );

//fonction qui vérifie le tag
AuthResult Guard_checkTag(char* idTag);

//fonction qui vérifie le résultat de la reconnaissance faciale
void Guard_resultRecognition(AuthResult authResult);

//fonction qui vérifie la tête de l'utilisateur
void Guard_checkFace(char* idTag);

#endif //GUARD_H