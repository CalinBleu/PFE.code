#ifndef ARCHIVIST_H
#define ARCHIVIST_H

char* Archivist_getName(char* idtag);

char* Archivist_getFirstName(char* idtag);

Role Archivist_getRole(char* idtag);

Access* Archivist_getAccess(char* idtag);

char* Archivist_getPassword();

Picture Archivist_getPicture(char* idtag);

char** Archivist_getTags(char* name);

#endif //ARCHIVIST_H