#include "sha256.h"

#include <stdio.h>
#include <string.h>

#define NB_LOCK 1

typedef enum AuthResult {
    ADMIN_TAG = 0,
    USER_TAG_OK,
    USER_TAG_UNKNOWN,
    USER_TAG_DENIED,
    ALLOWED,
    FACE_UNKNOWN
}AuthResult;


typedef enum Role {
    USER = 0,
    ADMIN
}Role;

const int zone = 0;

typedef bool Access[NB_LOCK];

typedef char Picture;

void checkPassword(char* password, char* idTag ){

    char password_hash[SHA256_HEX_SIZE];

    const char hash[SHA256_HEX_SIZE] = "26429a356b1d25b7d57c0f9a6d5fed8a290cb42374185887dcd2874548df0779";

    sha256_hex(password, strlen(password), password_hash);

    if(strcmp(password_hash, hash)== 0){
        printf("Correct Password\n");
    }
    else{
        printf("Incorrect Password\n");
    }
}

AuthResult checkTag(char* idTag){
    Role role = getRole(idTag);
    Access access = getAccess(idTag);

    if(role == ADMIN){
        return ADMIN_TAG;
    }
    else if (role == USER)
    {
        if(access[zone1] == true){
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

void resultRecognition(AuthResult authResult){
    if(authResult == ALLOWED){
        faceAnalysed(true);
    }
    else{
        faceAnalysed(false);
    }
}

void checkFace(char* idTag){
    Picture picture = getPicture(idTag);
    startRecognition(picture);
}

int main(void){


    char str[100];
    printf("\nEntrer votre mdp :");
    gets(str);
    checkPassword(str, "EAPFDKEAFEA");


    return 0;
}



