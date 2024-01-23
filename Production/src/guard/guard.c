#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "common.h"
#include "sha256.h"
#include "guard.h"
#include "brain/brain.h"
#include "archivist/archivist.h"

void Guard_checkPassword(char* password, char* idTag ){

    char password_hash[SHA256_HEX_SIZE];

    char *hash = Archivist_getPassword(idTag);

    sha256_hex(password, strlen(password), password_hash);

    if(strcmp(password_hash, hash)== 0){
        printf("Correct Password\n");
        //GUI_validatePassword(true);
    }
    else{
        printf("Incorrect Password\n");
        //GUI_validatePassword(false);
    }
}

AuthResult Guard_checkTag(char* idTag){
    Role role = Archivist_getRole(idTag);
    Access* access = Archivist_getAccess(idTag);

    if(role == ADMIN){
        return ADMIN_TAG;
    }
    else if (role == USER)
    {
        if(*access[CURRENT_ZONE] == true){
            return USER_TAG_OK;
        }
        else{
            return USER_TAG_DENIED;
        }
    }
    else{
        return USER_TAG_UNKNOWN;
    }
    
}

void Guard_resultRecognition(AuthResult authResult){
    if(authResult == ALLOWED){
        Brain_faceAnalysed(true);
    }
    else{
        Brain_faceAnalysed(false);
    }
}

void Guard_checkFace(char* idTag){
    Picture picture = Archivist_getPicture(idTag);
    //startRecognition(picture);
}


