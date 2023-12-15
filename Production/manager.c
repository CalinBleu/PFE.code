#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "common.h"
#include "manager.h"
#include "archivist.h"


void Manager_addUser(User user){
    Archivist_setUser(user);
}

void Manager_removeUser(char* idTag){
    Archivist_deleteEmployee(idTag);
}

void Manager_searchUser(char* searchField){
    char **tags;
    tags = Archivist_getTags(searchField);
    User* user;


    for(int i=0;i<sizeof(tags);i++){
        Manager_getAllUsers(user->name);
    }
}

void Manager_modifyUser(User user, char* idTag){
    Archivist_setIdTag(idTag, user.idTag);
    Archivist_setName(user.idTag, user.name);
    Archivist_setFirstName(user.idTag, user.firstName);
    Archivist_getPassword(user.idTag);
    Archivist_setPicture(user.idTag, user.picture);
    Archivist_setRole(user.idTag, user.role);
    Archivist_setAccess(user.idTag,&user.access);
}

int taille_char_array(char **array){
    int i = 0;
    while(array[i] != NULL){
        i++;
    }
    return i;
}   


User* Manager_getAllUsers(char* name){
    char **tags;

    tags = Archivist_getTags(name);

    int taille = taille_char_array(tags);

    User *users;
    users = (User*)malloc(taille * sizeof(User));

    for(int i = 0;i<taille;i++){
        users[i].name = Archivist_getName(tags[i]);
        users[i].firstName = Archivist_getFirstName(tags[i]);
        users[i].role = Archivist_getRole(tags[i]);
        *users[i].access = Archivist_getAccess(tags[i]);
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
    *user->access = Archivist_getAccess(idTag);
    user->idTag = idTag;
    user->picture = Archivist_getPicture(idTag);

    return user;
}


/*
int main(void){

    User user;
    user.name = "avczedvz";
    user.firstName = "acacae";
    user.role = USER;
    user.access[0] = true;
    user.idTag = "12222";
    user.picture = "image_test.jpg";

    Manager_addUser(user);

    return 0;

}
*/