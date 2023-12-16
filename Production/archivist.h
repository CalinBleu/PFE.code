#ifndef ARCHIVIST_H
#define ARCHIVIST_H

/**
 * @brief Récupère le nom d'un utilisateur à partir de son tag RFID
 * 
 * @param idtag la valeur du tag RFID lu
 * @return char* le nom de l'utilisateur
 */
char* Archivist_getName(char* idtag);

/**
 * @brief Récupère le prénom d'un utilisateur à partir de son tag RFID
 * 
 * @param idtag la valeur du tag RFID lu
 * @return char* le prénom de l'utilisateur
 */
char* Archivist_getFirstName(char* idtag);

/**
 * @brief Récupère la photo d'un utilisateur à partir de son tag RFID
 * 
 * @param idtag la valeur du tag RFID lu
 * @return Picture le chemin vers la photo téléchargée
 */
Picture Archivist_getPicture(char* idtag);

/**
 * @brief Récupère le rôle d'un utilisateur à partir de son tag RFID
 * 
 * @param idtag la valeur du tag RFID lu
 * @return char* le rôle de l'utilisateur
 */
Role Archivist_getRole(char* idtag);

/**
 * @brief Récupère les droits d'accès d'un utilisateur à partir de son tag RFID
 * 
 * @param idtag la valeur du tag RFID lu
 * @return char* le tableau des accès de l'utilisateur
 */
Access* Archivist_getAccess(char* idtag);

/**
 * @brief Récupère le mot de passe d'un administrateur à partir de son tag RFID
 * 
 * @param idtag la valeur du tag RFID lu
 * @return char* le mot de passe si l'utilisateur est un administrateur
 */
char* Archivist_getPassword(char* idtag);

/**
 * @brief Récupère les tags RFID des utilisateurs à partir de leur nom
 * 
 * @param name le nom recherché ou nom partiel ou paramètre nul si l'on veut récupérer tous les tags
 * @return char** l'ensemble des tags RFID trouvés
 */
char** Archivist_getTags(char* name);

/**
 * @brief Définit un nom pour un tag RFID donné
 * 
 * @param idtag la valeur du tag RFID de l'utilisateur à modifier
 * @param name le nom à définir
 */
void Archivist_setName(char* idtag, char* name);

/**
 * @brief Définit un prénom pour un tag RFID donné
 * 
 * @param idtag la valeur du tag RFID de l'utilisateur à modifier
 * @param firstname le prénom à définir
 */
void Archivist_setFirstName(char* idtag, char* firstname);

/**
 * @brief Définit une photo pour un tag RFID donné
 * 
 * @param idtag la valeur du tag RFID de l'utilisateur à modifier
 * @param picture le chemin vers la photo à charger
 */
void Archivist_setPicture(char* idtag, Picture picture);

/**
 * @brief Définit un rôle pour un tag RFID donné
 * 
 * @param idtag la valeur du tag RFID de l'utilisateur à modifier
 * @param role le rôle à définir
 */
void Archivist_setRole(char* idtag, Role role);

/**
 * @brief Définit les droits d'accès pour un tag RFID donné
 * 
 * @param idtag la valeur du tag RFID de l'utilisateur à modifier
 * @param access le tableau des accès à définir
 */
void Archivist_setAccess(char* idtag, Access* access);

/**
 * @brief Définit un nouveau tag pour un tag RFID donné
 * 
 * @param oldtag la valeur du tag RFID de l'utilisateur à modifier
 * @param newtag le tag RFID à définir
 */
void Archivist_setIdTag(char* oldtag, char* newtag);

/**
 * @brief Définit un mot de passe pour un tag RFID donné
 * 
 * @param idtag la valeur du tag RFID de l'administrateur à modifier
 * @param password le mot de passe à définir
 */
void Archivist_setPassword(char* idtag, char* password);

/**
 * @brief Crée une nouvelle entrée correspondant à l'utilisateur donné
 * 
 * @param user l'utilisateur à charger dans la base de données
 */
void Archivist_setUser(User user);

/**
 * @brief Supprime une entrée de la base de données pour un tag RFID donné
 * 
 * @param idtag la valeur du tag RFID de l'utilisateur à supprimer
 */
void Archivist_deleteEmployee(char* idtag);

/**
 * @brief Supprime les photos téléchargées dans le dossier Pictures servant d'intermédiaire entre la base de données et l'IHM/IA
 * 
 */
void Archivist_clearImages();

/**
 * @brief Récupère le nombre d'utilisateurs enregistrés
 * 
 * @param inputString le nom ou prénom partiel recherché ou paramètre vide si l'on veut récupérer tous les utilisateurs
 * @return int le nombre d'utilisateurs correspondant
 */
int Archivist_getNbEmployee(char* inputString);

#endif //ARCHIVIST_H