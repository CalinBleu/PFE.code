#include <sqlite3.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <math.h>
#include "common.h"
#include "archivist.h"

static void Archivist_clearImages();
void Archivist_deleteEmployee(char* idtag);

int main(void) {

    char* idtag = "1234";

    //getName
    char* name = Archivist_getName(idtag);

    if (name != NULL) {
        printf("Name: %s\n", name);
        free(name); 
    } else {
        fprintf(stderr, "No name found.\n");
    }

    //getFirstName
    char* firstname = Archivist_getFirstName(idtag);

    if (firstname != NULL) {
        printf("Firstname: %s\n", firstname);
        free(firstname); 
    } else {
        fprintf(stderr, "No firstname found.\n");
    }

    //getPicture
    Picture pic = Archivist_getPicture(idtag);

    if (pic != NULL)
    {
        printf("Picture path : %s\n", pic);
        free(pic);
    } else {
        fprintf(stderr, "No picture found.\n");
    }

    //getRole
    Role role = Archivist_getRole(idtag);

    if (role != -1) {
        printf("Role: %d\n", role);
    } else {
        fprintf(stderr, "No role found.\n");
    }

    //getAccess
    Access* access = Archivist_getAccess(idtag);

    if (access != NULL)
    {
        if (*access[CURRENT_ZONE] != false) {
            printf("Access granted\n");
        } else {
            printf("Access false\n");
        }
        free(access);
    } else {
        fprintf(stderr, "No access found.\n");
    }

    //getPassword (/!\ version mdp unique)
    char* pswd = Archivist_getPassword();

    if (pswd != NULL) {
        printf("Password: %s\n", pswd);
        free(pswd);
    } else {
        fprintf(stderr, "No password found.\n");
    }

    //getTags
    char* aname = "test";
    char** idtags = Archivist_getTags(aname);

    if (idtags != NULL) {
        for (int i = 0; idtags[i] != NULL; ++i) {
            printf("IDTAG %d: %s\n", i + 1, idtags[i]);
        }
    } else {
        fprintf(stderr, "No tags found\n");
    }

    if (idtags != NULL) {
        for (int i = 0; idtags[i] != NULL; ++i) {
            free(idtags[i]);
        }
        free(idtags);
    }

    //set
    Archivist_setName("1234", "Chiron");
    Archivist_setFirstName("1234", "Paul");
    //Archivist_setPicture("1234", "image_test.jpg");
    Archivist_setRole("1234", 1);
    Access newAccess;
    newAccess[CURRENT_ZONE] = false;
    Archivist_setAccess("1234", &newAccess);
    //Archivist_setIdTag("1234", "9999");

    //setUser et delete
    User new;
    new.firstName = "aaa";
    new.name = "bbb";
    new.picture = "image_test.jpg";
    new.role = ADMIN;
    new.idTag = "1240";
    new.access[CURRENT_ZONE] = false;

    Archivist_setUser(new);

    Archivist_deleteEmployee("1240");

    //clean le dossier Pictures
    Archivist_clearImages();

    return 0;
}

char* Archivist_getName(char* idtag) {

    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;
    
    int rc = sqlite3_open("visiolock.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return NULL;
    }
    
    char *sql = "SELECT * FROM Employee WHERE IdTag = ?"; 

    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc == SQLITE_OK) { 
        sqlite3_bind_text(res, 1, idtag, -1, SQLITE_STATIC);
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    
    int step = sqlite3_step(res);
    
    if (step == SQLITE_ROW) {
        size_t len = strlen((const char*)sqlite3_column_text(res, NAME));

        char *result = (char*)malloc(len + 1);

        if (result == NULL) {
            fprintf(stderr, "Memory allocation error\n");
            sqlite3_finalize(res);
            sqlite3_close(db);
            return NULL;
        }

        strcpy(result, (const char*)sqlite3_column_text(res, NAME));

        sqlite3_finalize(res);
        sqlite3_close(db);
        return result;
    }
    else
    {
        sqlite3_finalize(res);
        sqlite3_close(db);
        return NULL;
    }
    
}

char* Archivist_getFirstName(char* idtag) {

    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;
    
    int rc = sqlite3_open("visiolock.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return NULL;
    }
    
    char *sql = "SELECT * FROM Employee WHERE IdTag = ?"; 

    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc == SQLITE_OK) { 
        sqlite3_bind_text(res, 1, idtag, -1, SQLITE_STATIC);
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    
    int step = sqlite3_step(res);
    
    if (step == SQLITE_ROW) {
        size_t len = strlen((const char*)sqlite3_column_text(res, FIRSTNAME));

        char *result = (char*)malloc(len + 1);

        if (result == NULL) {
            fprintf(stderr, "Memory allocation error\n");
            sqlite3_finalize(res);
            sqlite3_close(db);
            return NULL;
        }

        strcpy(result, (const char*)sqlite3_column_text(res, FIRSTNAME));

        sqlite3_finalize(res);
        sqlite3_close(db);
        return result;
    }
    else
    {
        sqlite3_finalize(res);
        sqlite3_close(db);
        return NULL;
    }
    
}

Picture Archivist_getPicture(char* idtag) {

    char* path = "Pictures/";

    char* name = Archivist_getName(idtag);
    char* firstname = Archivist_getFirstName(idtag);

    size_t path_size = strlen(name) + strlen(firstname) + strlen(path) + strlen("_.jpg") + 1;
    Picture result = malloc(path_size);

    //à changer
    if (name != NULL) {
        if(firstname != NULL) {
            snprintf(result, path_size, "%s%s%s%s%s", path, firstname, "_", name, ".jpg");
            printf("path : %s\n", result);
            free(name);
            free(firstname);
        }
        else {
            fprintf(stderr, "No firstname found.\n");
            free(name);
            return NULL;
        }
    } else {
        fprintf(stderr, "No name found.\n");
        return NULL;
    }

    FILE *fp = fopen(result, "wb");
    
    if (fp == NULL) {
        
        fprintf(stderr, "Cannot open image file\n");    
        
        return NULL;
    }    
    
    sqlite3 *db;
    char *err_msg = 0;
    
    int rc = sqlite3_open("visiolock.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return NULL;
    }
    
    char *sql = "SELECT Picture FROM Employee WHERE IdTag = ?";
        
    sqlite3_stmt *pStmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &pStmt, 0);
    
    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "Failed to prepare statement\n");
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        
        sqlite3_close(db);
        
        return NULL;
    } 

    if (rc == SQLITE_OK) { 
        sqlite3_bind_text(pStmt, 1, idtag, -1, SQLITE_STATIC);
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    
    rc = sqlite3_step(pStmt);
    
    int bytes = 0;
    
    if (rc == SQLITE_ROW) {

        bytes = sqlite3_column_bytes(pStmt, 0);
    }
        
    fwrite(sqlite3_column_blob(pStmt, 0), bytes, 1, fp);

    if (ferror(fp)) {            
        
        fprintf(stderr, "fwrite() failed\n");

        return NULL;      
    }  
    
    int r = fclose(fp);

    if (r == EOF) {
        fprintf(stderr, "Cannot close file handler\n");
    }       
    
    rc = sqlite3_finalize(pStmt);   

    sqlite3_close(db);
    
    return result;
}

Role Archivist_getRole(char* idtag) {

    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;
    
    int rc = sqlite3_open("visiolock.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return -1;
    }
    
    char *sql = "SELECT * FROM Employee WHERE IdTag = ?"; 

    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc == SQLITE_OK) { 
        sqlite3_bind_text(res, 1, idtag, -1, SQLITE_STATIC);
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    
    int step = sqlite3_step(res);
    
    if (step == SQLITE_ROW) {
        size_t len = strlen((const char*)sqlite3_column_text(res, ROLE));

        char *result = (char*)malloc(len + 1);

        if (result == NULL) {
            fprintf(stderr, "Memory allocation error\n");
            sqlite3_finalize(res);
            sqlite3_close(db);
            return -1;
        }

        strcpy(result, (const char*)sqlite3_column_text(res, ROLE));
        Role role = strtol(result, NULL, 10);

        sqlite3_finalize(res);
        sqlite3_close(db);
        free(result);
        return role;
    }
    else
    {
        sqlite3_finalize(res);
        sqlite3_close(db);
        return -1;
    }
}


Access* Archivist_getAccess(char* idtag) {

    Access* rsl = malloc(NB_LOCK);

    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;
    
    int rc = sqlite3_open("visiolock.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return false;
    }
    
    char *sql = "SELECT * FROM Employee WHERE IdTag = ?"; 

    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc == SQLITE_OK) { 
        sqlite3_bind_text(res, 1, idtag, -1, SQLITE_STATIC);
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    
    int step = sqlite3_step(res);
    
    if (step == SQLITE_ROW) {
        size_t len = strlen((const char*)sqlite3_column_text(res, ACCESS));

        char *result = (char*)malloc(len + 1);

        if (result == NULL) {
            fprintf(stderr, "Memory allocation error\n");
            sqlite3_finalize(res);
            sqlite3_close(db);
            free(rsl);
            return NULL;
        }

        strcpy(result, (const char*)sqlite3_column_text(res, ACCESS));

        sqlite3_finalize(res);
        sqlite3_close(db);

        int access = strtol(result, NULL, 10);

        for(int i = 0; i < NB_LOCK; i++)
        {
            *rsl[i] = (access & (1 << i)) != 0;
        }
        
        free(result);
        return rsl;
    } else {
        sqlite3_finalize(res);
        sqlite3_close(db);
        free(rsl);
        return NULL;
    }
}

char* Archivist_getPassword() {

    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;
    
    int rc = sqlite3_open("visiolock.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return NULL;
    }
    
    char *sql = "SELECT * FROM Password"; 

    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    
    int step = sqlite3_step(res);
    
    if (step == SQLITE_ROW) {
        size_t len = strlen((const char*)sqlite3_column_text(res, 0));

        char *result = (char*)malloc(len + 1);

        if (result == NULL) {
            fprintf(stderr, "Memory allocation error\n");
            sqlite3_finalize(res);
            sqlite3_close(db);
            return NULL;
        }

        strcpy(result, (const char*)sqlite3_column_text(res, 0));

        sqlite3_finalize(res);
        sqlite3_close(db);
        return result;
    }
    else
    {
        sqlite3_finalize(res);
        sqlite3_close(db);
        return NULL;
    }
}

char** Archivist_getTags(char* name) {

    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;

    char** result;
    
    int rc = sqlite3_open("visiolock.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return NULL;
    }
    
    char* sql;
    if(name == NULL)
    {
        sql = "SELECT * FROM Employee";
    }
    else
    {
        sql = "SELECT * FROM Employee WHERE Name = ?"; 
    }

    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc == SQLITE_OK) { 
        if(name != NULL)
        {
            sqlite3_bind_text(res, 1, name, -1, SQLITE_STATIC);
        }
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    
    char** results = NULL;
    int resultCount = 0;

    while (sqlite3_step(res) == SQLITE_ROW) {

        const char* idtagValue = (const char*)sqlite3_column_text(res, IDTAG);
        size_t len = strlen(idtagValue);

        char* result = (char*)malloc(len + 1);
        if (result == NULL) {
            fprintf(stderr, "Memory allocation error\n");

            for (int i = 0; i < resultCount; ++i) {
                free(results[i]);
            }
            free(results);

            sqlite3_finalize(res);
            sqlite3_close(db);
            return NULL;
        }

        strcpy(result, idtagValue);

        results = (char**)realloc(results, (resultCount + 2) * sizeof(char*));
        if (results == NULL) {
            fprintf(stderr, "Memory allocation error\n");

            for (int i = 0; i < resultCount; ++i) {
                free(results[i]);
            }
            free(result);

            sqlite3_finalize(res);
            sqlite3_close(db);
            return NULL;
        }

        results[resultCount] = result;
        resultCount++;
    }

    sqlite3_finalize(res);
    sqlite3_close(db);

    if(resultCount)
    {
        results[resultCount] = NULL;
    }

    return results;
}

void Archivist_setName(char* idtag, char* name)
{
    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;
    
    int rc = sqlite3_open("visiolock.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return;
    }
    
    char *sql = "UPDATE Employee SET Name = ? WHERE IdTag = ?"; 

    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc == SQLITE_OK) { 
        sqlite3_bind_text(res, 1, name, -1, SQLITE_STATIC);
        sqlite3_bind_text(res, 2, idtag, -1, SQLITE_STATIC);
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    
    int step = sqlite3_step(res);

    if (step == SQLITE_DONE) {
        int rowsAffected = sqlite3_changes(db);
        if (rowsAffected > 0) {
            printf("Name updated successfully. Rows affected: %d\n", rowsAffected);
        } else {
            printf("No matching row found for update.\n");
        }
    } else {
        fprintf(stderr, "Error during name update: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(res);
    sqlite3_close(db);
}

void Archivist_setFirstName(char* idtag, char* firstname)
{
    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;
    
    int rc = sqlite3_open("visiolock.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return;
    }
    
    char *sql = "UPDATE Employee SET Firstname = ? WHERE IdTag = ?"; 

    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc == SQLITE_OK) { 
        sqlite3_bind_text(res, 1, firstname, -1, SQLITE_STATIC);
        sqlite3_bind_text(res, 2, idtag, -1, SQLITE_STATIC);
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    
    int step = sqlite3_step(res);

    if (step == SQLITE_DONE) {
        int rowsAffected = sqlite3_changes(db);
        if (rowsAffected > 0) {
            printf("Firstname updated successfully. Rows affected: %d\n", rowsAffected);
        } else {
            printf("No matching row found for update.\n");
        }
    } else {
        fprintf(stderr, "Error during firstname update: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(res);
    sqlite3_close(db);
}

void Archivist_setPicture(char* idtag, Picture picture)
{
    FILE *fp = fopen(picture, "rb");
    
    if (fp == NULL) {
        
        fprintf(stderr, "Cannot open image file\n");
        return; 
    }
        
    fseek(fp, 0, SEEK_END);
    
    if (ferror(fp)) {
        
        fprintf(stderr, "fseek() failed\n");
        int r = fclose(fp);

        if (r == EOF) {
            fprintf(stderr, "Cannot close file handler\n");          
        }    
    }  
    
    int flen = ftell(fp);
    
    if (flen == -1) {
        
        perror("error occurred");
        int r = fclose(fp);

        if (r == EOF) {
            fprintf(stderr, "Cannot close file handler\n");
        }    
    }
    
    fseek(fp, 0, SEEK_SET);
    
    if (ferror(fp)) {
        
        fprintf(stderr, "fseek() failed\n");
        int r = fclose(fp);

        if (r == EOF) {
            fprintf(stderr, "Cannot close file handler\n");
        }    
    }

    char data[flen+1];

    int size = fread(data, 1, flen, fp);
    
    if (ferror(fp)) {
        
        fprintf(stderr, "fread() failed\n");
        int r = fclose(fp);

        if (r == EOF) {
            fprintf(stderr, "Cannot close file handler\n");
        }   
    }
    
    int r = fclose(fp);

    if (r == EOF) {
        fprintf(stderr, "Cannot close file handler\n");
    }  

    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;
    
    int rc = sqlite3_open("visiolock.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return;
    }
    
    char *sql = "UPDATE Employee SET Picture = ? WHERE IdTag = ?"; 

    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc == SQLITE_OK) { 
        sqlite3_bind_blob(res, 1, data, size, SQLITE_STATIC);  
        sqlite3_bind_text(res, 2, idtag, -1, SQLITE_STATIC);
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    
    int step = sqlite3_step(res);

    if (step == SQLITE_DONE) {
        int rowsAffected = sqlite3_changes(db);
        if (rowsAffected > 0) {
            printf("Picture updated successfully. Rows affected: %d\n", rowsAffected);
        } else {
            printf("No matching row found for update.\n");
        }
    } else {
        fprintf(stderr, "Error during picture update: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(res);
    sqlite3_close(db); 
}

void Archivist_setRole(char* idtag, Role role)
{
    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;
    
    int rc = sqlite3_open("visiolock.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return;
    }
    
    char *sql = "UPDATE Employee SET Role = ? WHERE IdTag = ?"; 

    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc == SQLITE_OK) { 
        sqlite3_bind_int(res, 1, role);
        sqlite3_bind_text(res, 2, idtag, -1, SQLITE_STATIC);
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    
    int step = sqlite3_step(res);

    if (step == SQLITE_DONE) {
        int rowsAffected = sqlite3_changes(db);
        if (rowsAffected > 0) {
            printf("Role updated successfully. Rows affected: %d\n", rowsAffected);
        } else {
            printf("No matching row found for update.\n");
        }
    } else {
        fprintf(stderr, "Error during role update: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(res);
    sqlite3_close(db);
}

void Archivist_setAccess(char* idtag, Access* access)
{
    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;

    int accessIntValue = 0;
    for (int i = 0; i < NB_LOCK; i++) {
        if(*access[i] == true)
        {
            accessIntValue += (int)pow(2, i);
        }
    }
    
    int rc = sqlite3_open("visiolock.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return;
    }
    
    char *sql = "UPDATE Employee SET Access = ? WHERE IdTag = ?"; 

    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc == SQLITE_OK) { 
        sqlite3_bind_int(res, 1, accessIntValue);
        sqlite3_bind_text(res, 2, idtag, -1, SQLITE_STATIC);
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    
    int step = sqlite3_step(res);

    if (step == SQLITE_DONE) {
        int rowsAffected = sqlite3_changes(db);
        if (rowsAffected > 0) {
            printf("Access updated successfully. Rows affected: %d\n", rowsAffected);
        } else {
            printf("No matching row found for update.\n");
        }
    } else {
        fprintf(stderr, "Error during access update: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(res);
    sqlite3_close(db);
}

void Archivist_setIdTag(char* oldtag, char* newtag)
{
    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;
    
    int rc = sqlite3_open("visiolock.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return;
    }
    
    char *sql = "UPDATE Employee SET IdTag = ? WHERE IdTag = ?"; 

    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc == SQLITE_OK) { 
        sqlite3_bind_text(res, 1, newtag, -1, SQLITE_STATIC);
        sqlite3_bind_text(res, 2, oldtag, -1, SQLITE_STATIC);
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    
    int step = sqlite3_step(res);

    if (step == SQLITE_DONE) {
        int rowsAffected = sqlite3_changes(db);
        if (rowsAffected > 0) {
            printf("IdTag updated successfully. Rows affected: %d\n", rowsAffected);
        } else {
            printf("No matching row found for update.\n");
        }
    } else {
        fprintf(stderr, "Error during idtag update: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(res);
    sqlite3_close(db);
}

void Archivist_setUser(User user)
{
    char* name = user.name;
    char* firstname = user.firstName;
    Role role = user.role;
    Access access;
    char* idtag = user.idTag;
    Picture picture = user.picture;

    for (int i = 0; i < NB_LOCK; ++i) {
        access[i] = user.access[i];
    }

    int accessIntValue = 0;
    for (int i = 0; i < NB_LOCK; ++i) {
        if(access[i] == true)
        {
            accessIntValue += (int)pow(2, i);
        }
    }


    FILE *fp = fopen(picture, "rb");
    
    if (fp == NULL) {
        
        fprintf(stderr, "Cannot open image file\n");
        return; 
    }
        
    fseek(fp, 0, SEEK_END);
    
    if (ferror(fp)) {
        
        fprintf(stderr, "fseek() failed\n");
        int r = fclose(fp);

        if (r == EOF) {
            fprintf(stderr, "Cannot close file handler\n");          
        }    
    }  
    
    int flen = ftell(fp);
    
    if (flen == -1) {
        
        perror("error occurred");
        int r = fclose(fp);

        if (r == EOF) {
            fprintf(stderr, "Cannot close file handler\n");
        }    
    }
    
    fseek(fp, 0, SEEK_SET);
    
    if (ferror(fp)) {
        
        fprintf(stderr, "fseek() failed\n");
        int r = fclose(fp);

        if (r == EOF) {
            fprintf(stderr, "Cannot close file handler\n");
        }    
    }

    char data[flen+1];

    int size = fread(data, 1, flen, fp);
    
    if (ferror(fp)) {
        
        fprintf(stderr, "fread() failed\n");
        int r = fclose(fp);

        if (r == EOF) {
            fprintf(stderr, "Cannot close file handler\n");
        }   
    }
    
    int r = fclose(fp);

    if (r == EOF) {
        fprintf(stderr, "Cannot close file handler\n");
    }  

    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;
    
    int rc = sqlite3_open("visiolock.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return;
    }
    
    char *sql = "INSERT INTO Employee VALUES(?, ?, ?, ?, ?, ?);";

    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc == SQLITE_OK) { 
        sqlite3_bind_text(res, 1, firstname, -1, SQLITE_STATIC);
        sqlite3_bind_text(res, 2, name, -1, SQLITE_STATIC);
        sqlite3_bind_blob(res, 3, data, size, SQLITE_STATIC);  
        sqlite3_bind_text(res, 4, idtag, -1, SQLITE_STATIC);
        sqlite3_bind_int(res, 5, role);
        sqlite3_bind_int(res, 6, accessIntValue);
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    
    int step = sqlite3_step(res);

    if (step == SQLITE_DONE) {
        int rowsAffected = sqlite3_changes(db);
        if (rowsAffected > 0) {
            printf("User added successfully. Rows affected: %d\n", rowsAffected);
        } else {
            printf("No row has been added.\n");
        }
    } else {
        fprintf(stderr, "Error during insertion: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(res);
    sqlite3_close(db);
    
}

void Archivist_deleteEmployee(char* idtag)
{
    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;
    
    int rc = sqlite3_open("visiolock.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return;
    }
    
    char *sql = "DELETE FROM Employee WHERE IdTag = ?"; 

    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc == SQLITE_OK) { 
        sqlite3_bind_text(res, 1, idtag, -1, SQLITE_STATIC);
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    
    int step = sqlite3_step(res);

    if (step == SQLITE_DONE) {
        int rowsAffected = sqlite3_changes(db);
        if (rowsAffected > 0) {
            printf("User deleted successfully. Rows affected: %d\n", rowsAffected);
        } else {
            printf("No matching row found for deletion.\n");
        }
    } else {
        fprintf(stderr, "Error during deletion: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(res);
    sqlite3_close(db);
}

static void Archivist_clearImages()
{
    DIR *dir;
    struct dirent *entry;

    dir = opendir("/home/paul/Documents/S9/PFE/PFE.code/Explo/Explo_Paul/BDD/Pictures");

    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char filePath[512];
        snprintf(filePath, sizeof(filePath), "%s/%s", "/home/paul/Documents/S9/PFE/PFE.code/Explo/Explo_Paul/BDD/Pictures", entry->d_name);

        if (unlink(filePath) != 0) {
            perror("Error removing file");
        } else {
            printf("Removed file: %s\n", filePath);
        }
    }

    closedir(dir);
}
