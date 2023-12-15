#ifndef MANAGER_H
#define MANAGER_H

void Manager_addUser(User user);

void Manager_removeUser(char* idTag);

void Manager_searchUser(char* searchField);

void Manager_modifyUser(User user, char* idTag);

User* Manager_getAllUsers();

static User* Manager_getUserByTag(char* idTag);

#endif //MANAGER_H