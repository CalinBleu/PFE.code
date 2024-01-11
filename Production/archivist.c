#include <sqlite3.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <math.h>
#include "common.h"
#include "sha256.h"
#include "archivist.h"

sqlite3 *db;
int rc;

void Archivist_open()
{
    rc = sqlite3_open("visiolock.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
    }
}

void Archivist_close()
{
    sqlite3_close(db);
}

char* Archivist_getName(char* idtag) {

    char *err_msg = 0;
    sqlite3_stmt *res;
    
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
        return result;
    }
    else
    {
        sqlite3_finalize(res);
        return NULL;
    }
    
}

char* Archivist_getFirstName(char* idtag) {

    char *err_msg = 0;
    sqlite3_stmt *res;
    
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
        return result;
    }
    else
    {
        sqlite3_finalize(res);
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
            //printf("path : %s\n", result);
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

    char *err_msg = 0;
    
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
    
    return result;
}

Role Archivist_getRole(char* idtag) {

    char *err_msg = 0;
    sqlite3_stmt *res;
    
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
        free(result);
        return role;
    }
    else
    {
        sqlite3_finalize(res);
        return -1;
    }
}

Access* Archivist_getAccess(char* idtag) {

    Access* rsl = malloc(NB_LOCK);

    char *err_msg = 0;
    sqlite3_stmt *res;
    
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

        int access = strtol(result, NULL, 10);

        for(int i = 0; i < NB_LOCK; i++)
        {
            *rsl[i] = (access & (1 << i)) != 0;
        }
        
        free(result);
        return rsl;
    } else {
        sqlite3_finalize(res);
        free(rsl);
        return NULL;
    }
}

char* Archivist_getPassword(char* idtag) {

    char *err_msg = 0;
    sqlite3_stmt *res;
    
    char *sql = "SELECT * FROM Employee WHERE IdTag = ?"; 

    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc == SQLITE_OK) { 
        sqlite3_bind_text(res, 1, idtag, -1, SQLITE_STATIC);
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    
    int step = sqlite3_step(res);

    if (sqlite3_column_type(res, PASSWORD) == SQLITE_NULL)
    {
        sqlite3_finalize(res);
        return NULL;
    }
    
    if (step == SQLITE_ROW) {
        size_t len = strlen((const char*)sqlite3_column_text(res, PASSWORD));

        if(len == 0)
        {
            return NULL;
        }

        char *result = (char*)malloc(len + 1);

        if (result == NULL) {
            fprintf(stderr, "Memory allocation error\n");
            sqlite3_finalize(res);
            sqlite3_close(db);
            return NULL;
        }

        strcpy(result, (const char*)sqlite3_column_text(res, PASSWORD));

        sqlite3_finalize(res);
        return result;
    }
    else
    {
        sqlite3_finalize(res);
        return NULL;
    }
}

char** Archivist_getTags(char* name) {

    char *err_msg = 0;
    sqlite3_stmt *res;

    char** result;
    
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

    if(resultCount)
    {
        results[resultCount] = NULL;
    }

    return results;
}

void Archivist_setName(char* idtag, char* name)
{
    char *err_msg = 0;
    sqlite3_stmt *res;
    
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
}

void Archivist_setFirstName(char* idtag, char* firstname)
{
    char *err_msg = 0;
    sqlite3_stmt *res;
    
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

    char *err_msg = 0;
    sqlite3_stmt *res;
    
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
}

void Archivist_setRole(char* idtag, Role role)
{
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
}

void Archivist_setAccess(char* idtag, Access* access)
{
    char *err_msg = 0;
    sqlite3_stmt *res;

    int accessIntValue = 0;
    for (int i = 0; i < NB_LOCK; i++) {
        if(*access[i] == true)
        {
            accessIntValue += (int)pow(2, i);
        }
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
}

void Archivist_setIdTag(char* oldtag, char* newtag)
{
    char *err_msg = 0;
    sqlite3_stmt *res;
    
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
}

void Archivist_setPassword(char* idtag, char* password)
{
    char password_hash[SHA256_HEX_SIZE];
    sha256_hex(password, strlen(password), password_hash);

    char *err_msg = 0;
    sqlite3_stmt *res;
    
    char *sql = "UPDATE Employee SET Password = ? WHERE IdTag = ?"; 

    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc == SQLITE_OK) { 
        sqlite3_bind_text(res, 1, password_hash, -1, SQLITE_STATIC);
        sqlite3_bind_text(res, 2, idtag, -1, SQLITE_STATIC);
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    
    int step = sqlite3_step(res);

    if (step == SQLITE_DONE) {
        int rowsAffected = sqlite3_changes(db);
        if (rowsAffected > 0) {
            printf("Password updated successfully. Rows affected: %d\n", rowsAffected);
        } else {
            printf("No matching row found for update.\n");
        }
    } else {
        fprintf(stderr, "Error during password update: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(res);
}

void Archivist_setUser(User user)
{
    char* name = user.name;
    char* firstname = user.firstName;
    Role role = user.role;
    Access access;
    char* idtag = user.idTag;
    Picture picture = user.picture;
    char* password = user.password;

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

    char *err_msg = 0;
    sqlite3_stmt *res;
    
    char *sql = "INSERT INTO Employee VALUES(?, ?, ?, ?, ?, ?, ?);";

    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc == SQLITE_OK) { 
        sqlite3_bind_text(res, 1, firstname, -1, SQLITE_STATIC);
        sqlite3_bind_text(res, 2, name, -1, SQLITE_STATIC);
        sqlite3_bind_blob(res, 3, data, size, SQLITE_STATIC);  
        sqlite3_bind_text(res, 4, idtag, -1, SQLITE_STATIC);
        sqlite3_bind_int(res, 5, role);
        sqlite3_bind_int(res, 6, accessIntValue);
        sqlite3_bind_text(res, 7, password, -1, SQLITE_STATIC);
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
    
}

void Archivist_deleteEmployee(char* idtag)
{
    char *err_msg = 0;
    sqlite3_stmt *res;
    
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
}

void Archivist_clearImages()
{
    DIR *dir;
    struct dirent *entry;

    dir = opendir("Pictures/");

    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char filePath[512];
        snprintf(filePath, sizeof(filePath), "%s/%s", "Pictures", entry->d_name);

        if (unlink(filePath) != 0) {
            perror("Error removing file");
        } else {
            printf("Removed file: %s\n", filePath);
        }
    }

    closedir(dir);
}

int Archivist_getNbEmployee(char* inputString)
{
    int inputLength = 0;
    if(inputString != NULL)
    {
        inputLength = strlen(inputString);
    }
    char final[inputLength+2];
    if(inputString != NULL)
    {
        sprintf(final, "%c%s%c", '%',inputString,'%');
    }

    char *err_msg = 0;
    sqlite3_stmt *res;
    int count = 0;

    char* sql;
    if(inputString == NULL)
    {
        sql = "SELECT * FROM Employee";
    }
    else
    {
        sql = "SELECT * FROM Employee WHERE Name LIKE ? OR Firstname LIKE ?"; 
    }

    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc == SQLITE_OK) { 
        sqlite3_bind_text(res, 1, final, -1, SQLITE_STATIC);
        sqlite3_bind_text(res, 2, final, -1, SQLITE_STATIC);
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }

    while (sqlite3_step(res) == SQLITE_ROW) {
        count++;
    }

    sqlite3_finalize(res);

    return count;

}
