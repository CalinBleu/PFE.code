#ifndef MANAGER_H
#define MANAGER_H

#define NB_LOCK 1


typedef struct {
    char* name;
    char* firstName;
    Role role;
    Access access;
    char* idTag;
    Picture picture;
}User;

typedef enum Role{
    USER = 0,
    ADMIN

}Role;

typedef bool Access[NB_LOCK];

typedef char Picture;

void Manager_addUser(User user);
void Manager_removeUser(char* idTag);
void Manager_searchUser(char* searchField);
void Manager_modifyUser(User user);
User* Manager_getAllUsers();
User* Manager_getUserByTag(char* idTag);

#endif //MANAGER_H