#ifndef GUARD_H
#define GUARD_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

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


typedef struct {
    char* name;
    char* firstName;
    Role role;
    Access access;
    char* idTag;
    Picture picture;
}User;

void Guard_checkPassword(char* password, char* idTag );
AuthResult Guard_checkTag(char* idTag);
void Guard_resultRecognition(AuthResult authResult);
void Guard_checkFace(char* idTag);

#endif //GUARD_H