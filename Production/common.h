#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <errno.h>

#define NB_LOCK         1   //Nombre de serrures supportées par cette instance Visiolock
#define CURRENT_ZONE    0   //Indice de la serrure courante
#define TARGET          0   //Execution sur machine hôte / raspberry
#define COMMAND         "ps aux | grep 'R.* read.py' | awk '{print $2}' | head -n 1 | xargs kill -SIGTERM"

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

/**
 * @brief Enumération des identifiants de commande entre GUI et Visiolock
 * 
 */
typedef enum {
    CMD_ERR = 0,
    CMD_SCREEN_ON,
    CMD_SCREEN_OFF,
    CMD_DISPLAY_HOME_SCREEN,
    CMD_SET_TAG,
    CMD_CHANGE_MODE,
    CMD_STANDBY,
    CMD_CHECK_PASSWORD,
    CMD_VALIDATE_PASSWORD,
    CMD_SET_PICTURE,
    CMD_ADD_USER,
    CMD_MODIFY_USER,
    CMD_REMOVE_USER,
    CMD_ASK_SEARCH_USER,
    CMD_SET_SEARCH_USER,
    CMD_ASK_ALL_USERS,
    CMD_SET_ALL_USERS,
    NB_CMD
} GuiCommand;

#endif //COMMON_H