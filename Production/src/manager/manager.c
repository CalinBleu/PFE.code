#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "common.h"
#include "manager.h"
#include "archivist/archivist.h"

static int taille_char_array(char **array);

static void free_tags(char **tags);

 
/**
 * brief Fonction qui retourne un utilisateur de la base de données en fonction de son idTag
 * 
 * param idTag L'idTag de l'utilisateur à retourner
 */
static User* Manager_getUserByTag(char* idTag);

/**
 * brief Fonction qui ajoute un utilisateur dans la base de données
 * 
 * param user L'utilisateur à ajouter
 */
void Manager_addUser(User user){
    Archivist_setUser(user);
}

/**
 * brief Fonction qui supprime un utilisateur de la base de données
 * 
 * param idTag L'idTag de l'utilisateur à supprimer
 */
void Manager_removeUser(char* idTag){
    Archivist_deleteEmployee(idTag);
}

/**
 * brief Fonction qui recherche un utilisateur dans la base de données
 * 
 * param searchField Le champ de recherche
 */
User* Manager_searchUser(char* searchField){
    //on récupère le ou les tags liés au nom ou prénom entrés

    char **tags = (char**)malloc(sizeof(char*));

    tags = Archivist_getTags(searchField);

    int taille_tags = taille_char_array(tags);

    User* user = (User*)malloc(taille_tags * sizeof(User));

    //pour chaque tag trouvé, on récupère l'utilisateur associé
    for(int i=0;i<taille_tags;i++){
        User* current_user = Manager_getUserByTag(tags[i]);

        if(current_user != NULL){
            user[i] = *current_user;
        }
    }

    return user;
}

/**
 * brief Fonction qui modifie un utilisateur dans la base de données
 * 
 * param user L'utilisateur à modifier, idTag L'idTag de l'utilisateur à modifier
 */
void Manager_modifyUser(User user, char* idTag){
    Archivist_setIdTag(idTag, user.idTag);
    Archivist_setName(user.idTag, user.name);
    Archivist_setFirstName(user.idTag, user.firstName);
    if(user.password == NULL)
    {
        Archivist_setPassword(user.idTag, "");
    }
    else
    {
        Archivist_setPassword(user.idTag, user.password);
    }
    Archivist_setPicture(user.idTag, user.picture);
    Archivist_setRole(user.idTag, user.role);
    Archivist_setAccess(user.idTag,&user.access);
}

/**
 * brief Fonction qui retourne tous les utilisateurs de la base de données
 * 
 */
User* Manager_getAllUsers(){
    //on recherche tous les tags présents dans la BDD et on retourne les utilisateurs

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
       }
       free(current_user);
    }

    return users;
}

/**
 * brief Fonction qui retourne tous les tags de la base de données
 * 
 * param searchField Le champ de recherche
 */
static User* Manager_getUserByTag(char* idTag){

    User *user = (User*)malloc(sizeof(User));

    user->name = Archivist_getName(idTag);
    user->firstName = Archivist_getFirstName(idTag);
    user->role = Archivist_getRole(idTag);
    *user->access = Archivist_getAccess(idTag);
    user->idTag = idTag;
    user->picture = Archivist_getPicture(idTag);
    user->password = Archivist_getPassword(idTag);

    return user;
}

 
/**
 * brief Fonction qui retourne la taille d'un tableau de char*
 * 
 * param array Le tableau de char*
 */
static int taille_char_array(char **array){ 
    if(array == NULL)
    {
        return 0;
    }
    int i = 0;
    while(array[i] != NULL){
        i++;
    }
    return i;
}   

/**
 * brief Fonction qui libère la mémoire d'un tableau de char*
 * 
 * param array Le tableau de char*
 */
static void free_tags(char **tags){ 
    int i = 0;
    while(tags[i] != NULL){
        free(tags[i]);
        i++;
    }
    free(tags);
}
