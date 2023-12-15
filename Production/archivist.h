#ifndef ARCHIVIST_H
#define ARCHIVIST_H

char* Archivist_getName(char* idtag);

char* Archivist_getFirstName(char* idtag);

Picture Archivist_getPicture(char* idtag);

Role Archivist_getRole(char* idtag);

Access* Archivist_getAccess(char* idtag);

char* Archivist_getPassword(char* idtag);

char** Archivist_getTags(char* name);

void Archivist_setName(char* idtag, char* name);

void Archivist_setFirstName(char* idtag, char* firstname);

void Archivist_setPicture(char* idtag, Picture picture);

void Archivist_setRole(char* idtag, Role role);

void Archivist_setAccess(char* idtag, Access* access);

void Archivist_setIdTag(char* oldtag, char* newtag);

void Archivist_setPassword(char* idtag, char* password);

void Archivist_setUser(User user);

void Archivist_deleteEmployee(char* idtag);

#endif //ARCHIVIST_H