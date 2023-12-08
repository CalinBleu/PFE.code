#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "manager.h"

void Manager_addUser(User user){
    Archivist_setUser(user);
}

void Manager_removeUser(char* idTag){

}

void Manager_searchUser(char* searchField){
    char* tags[] = Archivist_getTags(searchField);
    User* user;


    for(int i=0;i<sizeof(tags);i++){

    }
}

void Manager_modifyUser(User user){
    Archivist_setName(user.name);
    Archivist_setFirstName(user.firstName);
    Archivist_setPicture(user.picture);
    Archivist_setRole(user.role);
    Archivist_setAccess(user.access);
    Archivist_setIdTag(user.idTag);
}

User* Manager_getAllUsers(){
    char* tags[] = Archivist_getTags();
    User users[sizeof(tags)];

    for(int i = 0;i<sizeof(tags);i++){
        users[i].name = Archivist_getName(tags[i]);
        users[i].firstName = Archivist_getFirstName(tags[i]);
        users[i].role = Archivist_getRole(tags[i]);
        users[i].access = Archivist_getAccess(tags[i]);
        users[i].idTag = tags[i];
        users[i].picture = Archivist_getPicture(tags[i]);
    }

    return users;
}

static User* Manager_getUserByTag(char* idTag){
    User *user;

    user->name = Archivist_getName(idTag);
    user->firstName = Archivist_getFirstName(idTag);
    user->role = Archivist_getRole(idTag);
    user->access = Archivist_getAccess(idTag);
    user->idTag = idTag;
    user->picture = Archivist_getPicture(idTag);

    return user;
}

int main(void){

}