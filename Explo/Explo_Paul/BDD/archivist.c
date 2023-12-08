#include <sqlite3.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

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

int main(void) {

    char* idtag = "1237";

    char* name = Archivist_getName(idtag);
    char* firstname = Archivist_getFirstName(idtag);
    Role role = Archivist_getRole(idtag);
    Access* access = Archivist_getAccess(idtag);
    Picture pic = Archivist_getPicture(idtag);

    if (name != NULL) {
        printf("Name: %s\n", name);
        free(name); 
    } else {
        fprintf(stderr, "No name found.\n");
    }

    if (firstname != NULL) {
        printf("Firstname: %s\n", firstname);
        free(firstname); 
    } else {
        fprintf(stderr, "No firstname found.\n");
    }
    if (role != -1) {
        printf("Role: %d\n", role);
    } else {
        fprintf(stderr, "No role found.\n");
    }

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

    if (pic != NULL)
    {
        printf("Picture path : %s\n", pic);
        free(pic);
    } else {
        fprintf(stderr, "No picture found.\n");
    }

    char* pswd = Archivist_getPassword();
    if (pswd != NULL) {
        printf("Password: %s\n", pswd);
        free(pswd);
    } else {
        fprintf(stderr, "No password found.\n");
    }

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

    Picture result = NULL;
    char* path = "Pictures/";

    char* name = Archivist_getName(idtag);
    char* firstname = Archivist_getFirstName(idtag);

    if (name != NULL) {
        //result = strcat(path, )
        free(name); 
    } else {
        fprintf(stderr, "No name found.\n");
        return NULL;
    }

    if (firstname != NULL) {
        printf("Firstname: %s\n", firstname);
        free(firstname); 
    } else {
        fprintf(stderr, "No firstname found.\n");
    }

    FILE *fp = fopen("Pictures/result.jpg", "wb");
    
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
        printf("access : %d\n", access);

        for(int i = 0; i < NB_LOCK; i++)
        {
            *rsl[i] = access%(2^i);
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
    
}
