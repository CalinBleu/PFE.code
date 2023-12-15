#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "common.h"
#include "manager.h"
#include "archivist.h"

int nombreUtilisateurs = 0; //à retirer plus tard



//fonction qui retourne la taille d'un tableau de char*
int taille_char_array(char **array){ 
    int i = 0;
    while(array[i] != NULL){
        i++;
    }
    return i;
}   

//fonction qui libère la mémoire d'un tableau de char*
void free_tags(char **tags){ 
    int i = 0;
    while(tags[i] != NULL){
        free(tags[i]);
        i++;
    }
    free(tags);
}

//fonction qui ajoute un utilisateur dans la base de données
void Manager_addUser(User user){
    Archivist_setUser(user);
}

//fonction qui supprime un utilisateur de la base de données
void Manager_removeUser(char* idTag){
    Archivist_deleteEmployee(idTag);
}


//fonction qui recherche un utilisateur dans la base de données
User* Manager_searchUser(char* searchField){

    nombreUtilisateurs = 0; // à retirer plus tard

    char **tags = (char**)malloc(sizeof(char*));

    tags = Archivist_getTags(searchField);

    int taille_tags = taille_char_array(tags);

    User* user = (User*)malloc(taille_tags * sizeof(User));

    for(int i=0;i<taille_tags;i++){
        User* current_user = Manager_getUserByTag(tags[i]);

        if(current_user != NULL){
            user[i] = *current_user;
            nombreUtilisateurs++;
        }
    }

    return user;
}

//fonction qui modifie un utilisateur dans la base de données
void Manager_modifyUser(User user, char* idTag){
    Archivist_setIdTag(idTag, user.idTag);
    Archivist_setName(user.idTag, user.name);
    Archivist_setFirstName(user.idTag, user.firstName);
    Archivist_getPassword(user.idTag);
    Archivist_setPicture(user.idTag, user.picture);
    Archivist_setRole(user.idTag, user.role);
    Archivist_setAccess(user.idTag,&user.access);
}

//fonction qui retourne tous les utilisateurs de la base de données
User* Manager_getAllUsers(){

    nombreUtilisateurs = 0; // à retirer plus tard

    char** tags = (char**)malloc(sizeof(char*));

    tags = Archivist_getTags(NULL);

    int taille = taille_char_array(tags);

    User *users = (User*)malloc(taille * sizeof(User));

    if(users == NULL){
        printf("Erreur d'allocation mémoire\n");
        free_tags(tags);
        return NULL;
    }   

    for(int i = 0;i<taille;i++){
       User* current_user = Manager_getUserByTag(tags[i]);

       if (current_user != NULL)
       {
         users[i] = *current_user;
         nombreUtilisateurs++;
       }
       
    }

    return users;
}


//fonction qui retourne un utilisateur de la base de données en fonction de son idTag
static User* Manager_getUserByTag(char* idTag){

    User *user = (User*)malloc(sizeof(User));

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