#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "common.h"
#include "brain.h"
#include "archivist.h"
#include "manager.h"

int main(){

    //simulation scénario
    Brain_startVisiolock();

    sleep(2);
    Brain_tagReaded("1234");
    sleep(3);
    Brain_faceAnalysed(true);
    sleep(2);
    Brain_tagReaded("1235");
    sleep(5);


    Brain_free();
    
    /*
    User user;
    user.name = "Ravus";
    user.firstName = "atroce";
    user.role = ADMIN;
    user.access[0] = true;
    user.idTag = "000";
    user.picture = "image_test.jpg";

    //Manager_addUser(user);

    //Manager_removeUser("12222");

    //Manager_modifyUser(user, "111");


    User *user_get;

    user_get = Manager_getAllUsers(user.name);

    printf("%s\n", user_get[0].name);
    printf("%s\n", user_get[0].firstName);
    printf("%d\n", user_get[0].role);
    printf("%d\n", user_get[0].access[0]);
    printf("%s\n", user_get[0].idTag);
    printf("%s\n", user_get[0].picture);
    */

    Archivist_clearImages();
    return 0;
}
