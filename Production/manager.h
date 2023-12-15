#ifndef MANAGER_H
#define MANAGER_H

extern int nombreUtilisateurs;

int taille_char_array(char **array);

void free_tags(char **tags);

void Manager_addUser(User user);

void Manager_removeUser(char* idTag);

User* Manager_searchUser(char* searchField);

void Manager_modifyUser(User user, char* idTag);

User* Manager_getAllUsers();

static User* Manager_getUserByTag(char* idTag);

#endif //MANAGER_H