#include "common.h"
#include "sha256.h"
#include "guard.h"
#include "archivist.h"


void Guard_checkPassword(char* password, char* idTag ){

    char password_hash[SHA256_HEX_SIZE];

    char *hash = Archivist_getPassword(idTag);

    sha256_hex(password, strlen(password), password_hash);

    if(strcmp(password_hash, hash)== 0){
        printf("Correct Password\n");
    }
    else{
        printf("Incorrect Password\n");
    }
}

AuthResult Guard_checkTag(char* idTag){
    Role role = Archivist_getRole(idTag);
    Access* access = Archivist_getAccess(idTag);

    if(role == ADMIN){
        return ADMIN_TAG;
    }
    else if (role == USER)
    {
        if(*access[CURRENT_ZONE] == true){
            return USER_TAG_OK;
        }
        else{
            return USER_TAG_DENIED;
        }
    }
    else{
        return USER_TAG_UNKNOWN;
    }
    
}

/*
void Guard_resultRecognition(AuthResult authResult){
    if(authResult == ALLOWED){
        faceAnalysed(true);
    }
    else{
        faceAnalysed(false);
    }
}
*/
void Guard_checkFace(char* idTag){
    Picture picture = Archivist_getPicture(idTag);
    //startRecognition(picture);
}



/*
int main(void){

    
    char str[100];
    printf("\nEntrer votre mdp :");
    fgets(str, sizeof(str), stdin);
    Guard_checkPassword(str, "1234");
    return 0;
    
   
    int result = Guard_checkTag("1239");
    printf("%d\n", result);
}

*/



