#ifndef MANAGER_H
#define MANAGER_H

extern int nombreUtilisateurs;

static int taille_char_array(char **array);

static void free_tags(char **tags);

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


//fonction qui retourne un utilisateur de la base de données en fonction de son idTag
static User* Manager_getUserByTag(char* idTag);

#endif //MANAGER_H