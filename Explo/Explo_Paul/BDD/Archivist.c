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

typedef bool Access[NB_LOCK];

char* getName(char* idtag);
char* getFirstName(char* idtag);
Role getRole(char* idtag);
Access* getAccess(char* idtag);

int main(void) {

    char* idtag = "123";

    char* name = getName(idtag);
    char* firstname = getFirstName(idtag);
    Role role = getRole(idtag);
    Access* access = getAccess(idtag);

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

    return 0;
}

char* getName(char* idtag) {

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
        size_t len = strlen((const char*)sqlite3_column_text(res, 1));

        char *result = (char*)malloc(len + 1);

        if (result == NULL) {
            fprintf(stderr, "Memory allocation error\n");
            sqlite3_finalize(res);
            sqlite3_close(db);
            return NULL;
        }

        strcpy(result, (const char*)sqlite3_column_text(res, 1));

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

char* getFirstName(char* idtag) {

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

//Picture getPicture(char* idtag) {
//}

Role getRole(char* idtag) {

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
        size_t len = strlen((const char*)sqlite3_column_text(res, 4));

        char *result = (char*)malloc(len + 1);

        if (result == NULL) {
            fprintf(stderr, "Memory allocation error\n");
            sqlite3_finalize(res);
            sqlite3_close(db);
            return -1;
        }

        strcpy(result, (const char*)sqlite3_column_text(res, 4));
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


Access* getAccess(char* idtag) {

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
        size_t len = strlen((const char*)sqlite3_column_text(res, 5));

        char *result = (char*)malloc(len + 1);

        if (result == NULL) {
            fprintf(stderr, "Memory allocation error\n");
            sqlite3_finalize(res);
            sqlite3_close(db);
            free(rsl);
            return NULL;
        }

        strcpy(result, (const char*)sqlite3_column_text(res, 5));

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

char* getPassword() {

}
