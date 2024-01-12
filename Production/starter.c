#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "common.h"
#include "brain.h"
#include "archivist.h"
#include "manager.h"
#include "archivist.h"
#include "doorman.h"
#include "rfid.h"

int main(){

    Rfid_new();
    Rfid_start();
    Brain_startVisiolock();
    Doorman_init();
    Archivist_open();

    /*
    sleep(2);
    Brain_tagReaded("1234");
    sleep(3);
    Brain_changeMode(MODE_ADMIN);
    //Brain_faceAnalysed(false);
    sleep(2);
    Brain_tagReaded("1235");
    sleep(10);
    Brain_faceAnalysed(true);
    sleep(10);
    
    

    User user;
    user.name = "Ravus";
    user.firstName = "atroce";
    user.role = ADMIN;
    user.access[0] = true;
    user.idTag = "000";
    user.picture = "image_test.jpg";

    */
    
    User user2;
    user2.name = "Ravus";
    user2.firstName = "mabiche";
    user2.role = ADMIN;
    user2.access[0] = true;
    user2.idTag = "123";
    user2.picture = "image_test.jpg";

    //Manager_addUser(user);

    //Manager_removeUser("12222");

    //Manager_modifyUser(user, "111");
    
    /*
    User* user_get;

    user_get = Manager_getAllUsers();

    printf("%s\n",user_get[0].idTag);

    for(int i = 0; i<Archivist_getNbEmployee(NULL);i++){
        printf("User %d : %s\n", i, user_get[i].name);
        printf("User %d : %s\n", i, user_get[i].firstName);
        printf("User %d : %d\n", i, user_get[i].role);
        printf("User %d : %s\n", i, user_get[i].idTag);
        printf("User %d : %d\n", i, user_get[i].access[CURRENT_ZONE]);
    }


    free(user_get);    

    Archivist_clearImages();

    */

    User* user_search;

    user_search = Manager_searchUser(user2.name);
    
    for(int i = 0; i<Archivist_getNbEmployee(user2.name);i++){
        printf("User %d : %s\n", i, user_search[i].name);
        printf("User %d : %s\n", i, user_search[i].firstName);
        printf("User %d : %d\n", i, user_search[i].role);
        printf("User %d : %s\n", i, user_search[i].idTag);
        printf("User %d : %d\n", i, user_search[i].access[CURRENT_ZONE]);
        printf("User %d : %s\n", i, user_search[i].password);
    }
    

    Archivist_clearImages();

    Archivist_close();

    Brain_stopVisiolock();
    Rfid_stop();
    sleep(3);
    Brain_free();
    Rfid_free();
    

    /*
    int nbemployee = Archivist_getNbEmployee(NULL);
    printf("%d\n", nbemployee);
    */
    
    return 0;
}
