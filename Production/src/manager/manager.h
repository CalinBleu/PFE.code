#ifndef MANAGER_H
#define MANAGER_H


/**
 * brief Fonction qui ajoute un utilisateur dans la base de données
 * 
 * param user L'utilisateur à ajouter
 */
void Manager_addUser(User user);


/**
 * brief Fonction qui supprime un utilisateur de la base de données
 * 
 * param idTag L'idTag de l'utilisateur à supprimer
 */
void Manager_removeUser(char* idTag);


/**
 * brief Fonction qui recherche un utilisateur dans la base de données
 * 
 * param searchField Le champ de recherche
 */
User* Manager_searchUser(char* searchField);


/**
 * brief Fonction qui modifie un utilisateur dans la base de données
 * 
 * param user L'utilisateur à modifier, idTag L'idTag de l'utilisateur à modifier
 */
void Manager_modifyUser(User user, char* idTag);


/**
 * brief Fonction qui retourne tous les utilisateurs de la base de données
 * 
 */
User* Manager_getAllUsers();

#endif //MANAGER_H