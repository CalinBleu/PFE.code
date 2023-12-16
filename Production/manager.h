#ifndef MANAGER_H
#define MANAGER_H

extern int nombreUtilisateurs;

//fonction qui ajoute un utilisateur dans la base de données
void Manager_addUser(User user);

//fonction qui supprime un utilisateur de la base de données
void Manager_removeUser(char* idTag);

//fonction qui recherche un utilisateur dans la base de données
User* Manager_searchUser(char* searchField);

//fonction qui modifie un utilisateur dans la base de données
void Manager_modifyUser(User user, char* idTag);

//fonction qui retourne tous les utilisateurs de la base de données
User* Manager_getAllUsers();

#endif //MANAGER_H