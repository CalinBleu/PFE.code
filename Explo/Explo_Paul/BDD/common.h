#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>

//Nombre de serrures supportées par cette instance Visiolock
#define NB_LOCK 1
//Indice de la serrure courante
#define CURRENT_ZONE 0

/**
 * @brief Enumération correspondant aux différents rôles disponibles
 * 
 */
typedef enum 
{
    USER = 0,
    ADMIN
}Role;

/**
 * @brief Enumération correspondant aux indices des colonnes de la table SQL Employee
 * 
 */
enum
{
    FIRSTNAME = 0,
    NAME,
    PICTURE,
    IDTAG,
    ROLE,
    ACCESS
}ColumnId;

/**
 * @brief Enumération correspondant aux différents résultats d'authentification (RFID et reconaissance faciale)
 * 
 */
typedef enum AuthResult {
    ADMIN_TAG = 0,
    USER_TAG_OK,
    USER_TAG_UNKNOWN,
    USER_TAG_DENIED,
    ALLOWED,
    FACE_UNKNOWN
}AuthResult;

typedef struct User
{
    /* data */
}User;

typedef bool Access[NB_LOCK];
typedef char* Picture;

#endif //COMMON_H