#ifndef ARCHIVIST_H
#define ARCHIVIST_H

#include <sqlite3.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define NB_LOCK 1
#define CURRENT_ZONE 0

typedef enum 
{
    USER = 0,
    ADMIN
}Role;



typedef enum
{
    FIRSTNAME = 0,
    NAME,
    PICTURE,
    IDTAG,
    ROLE,
    ACCESS
}ColumnId;

typedef bool Access[NB_LOCK];
typedef char* Picture;

char* Archivist_getName(char* idtag);
char* Archivist_getFirstName(char* idtag);
Role Archivist_getRole(char* idtag);
Access* Archivist_getAccess(char* idtag);
char* Archivist_getPassword();
Picture Archivist_getPicture(char* idtag);
char** Archivist_getTags(char* name);

#endif //ARCHIVIST_H

