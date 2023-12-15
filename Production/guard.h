#ifndef GUARD_H
#define GUARD_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void Guard_checkPassword(char* password, char* idTag );
AuthResult Guard_checkTag(char* idTag);
void Guard_resultRecognition(AuthResult authResult);
void Guard_checkFace(char* idTag);

#endif //GUARD_H