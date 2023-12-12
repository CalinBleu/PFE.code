#ifndef ARCHIVIST_H
#define ARCHIVIST_H

char* Archivist_getName(char* idtag);

char* Archivist_getFirstName(char* idtag);

Picture Archivist_getPicture(char* idtag);

Role Archivist_getRole(char* idtag);

Access* Archivist_getAccess(char* idtag);

char* Archivist_getPassword();

char** Archivist_getTags(char* name);

void Archivist_setName(char* name);

void Archivist_setFirstName(char* firstname);

void Archivist_setPicture(Picture picture);

void Archivist_setRole(Role role);

void Archivist_setAccess(Access access);

void Archivist_setIdTag(char* idTag);

void Archivist_setUser(User user);

#endif //ARCHIVIST_H