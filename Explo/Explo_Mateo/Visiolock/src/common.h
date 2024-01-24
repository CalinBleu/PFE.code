#ifndef COMMON_H
#define COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <errno.h>

#define NB_LOCK         1   //Nombre de serrures supportées par cette instance Visiolock
#define CURRENT_ZONE    0   //Indice de la serrure courante
#define TARGET          0   //Execution sur machine hôte / raspberry

/**
 * @brief Enumération correspondant aux indices des colonnes de la table SQL Employee
 * 
 */
typedef enum
{
    FIRSTNAME = 0,
    NAME,
    PICTURE,
    IDTAG,
    ROLE,
    ACCESS,
    PASSWORD
}ColumnId;

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
 * @brief Enumération correspondant aux différents modes disponibles
 * 
 */
typedef enum
{
    MODE_CLASSIC = 0,
    MODE_ADMIN
}Mode;

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

typedef bool Access[NB_LOCK];
typedef char* Picture;

/**
 * @brief Structure correpondant à un utilisateur général de Visiolock
 * 
 */
typedef struct {
    char* name;
    char* firstName;
    Role role;
    Access access;
    char* idTag;
    Picture picture;
    char* password;
}User;

#ifdef __cplusplus
}
#endif

#endif //COMMON_H
