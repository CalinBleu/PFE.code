#ifndef GUARD_H
#define GUARD_H

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

void checkPassword(char* password, char* idTag );
AuthResult checkTag(char* idTag);
void resultRecognition(AuthResult authResult);
void checkFace(char* idTag);

#endif //GUARD_H