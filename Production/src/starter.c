#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "common.h"
#include "brain/brain.h"
#include "archivist/archivist.h"
#include "manager/manager.h"
#include "doorman/doorman.h"
#include "rfid/rfid.h"
#include "ai/ai.h"
#include "gui/gui.h"

#define LIBRE           0
#define CONNEXION_KO    0
#define CONNEXION_OK    0
#define ADD_USER        0
#define MODIFY_USER     1
#define REMOVE_USER     0
#define SEARCH_USER     0

int main(){

    //démarrage de l'application
    Rfid_new();
    Rfid_start();
    AI_new();
    AI_start();
    Brain_startVisiolock();
    Doorman_init();
    Archivist_open();

    if(LIBRE)
    {
        sleep(60);
    }
    if(CONNEXION_KO)
    {
        //connexion avec un badge admin et mot de passe incorrect
        sleep(2);
        Brain_tagReaded("29416637637");
        sleep(3);
        Gui_askConnexion("1234");
        sleep(1);
    }
    if(CONNEXION_OK)
    {
        //connexion avec un badge admin et mot de passe correct
        sleep(2);
        Brain_tagReaded("29416637637");
        sleep(3);
        Gui_askConnexion("12345678");
        sleep(1);
        Gui_changeMode(MODE_ADMIN);
        sleep(1);
    }
    if(ADD_USER)
    {
        //connexion avec un badge admin et ajout d'un utilisateur
        sleep(2);
        Brain_tagReaded("29416637637");
        sleep(3);
        Gui_askConnexion("12345678");
        sleep(1);
        Gui_changeMode(MODE_ADMIN);
        sleep(1);

        User user;
        user.name = "ajout";
        user.firstName = "ajout";
        user.role = ADMIN;
        user.access[0] = true;
        user.idTag = "999999999999";
        user.picture = "Camera/last_shot.jpg";
        user.password = "12345678";

        Gui_userAdded(user);
        sleep(1);
    }
    if(MODIFY_USER)
    {
        //connexion avec un badge admin et modification d'un utilisateur
        sleep(2);
        Brain_tagReaded("29416637637");
        sleep(3);
        Gui_askConnexion("12345678");
        sleep(1);
        Gui_changeMode(MODE_ADMIN);
        sleep(1);

        User user;
        user.name = "modifie";
        user.firstName = "modifie";
        user.role = USER;
        user.access[0] = false;
        user.idTag = "888888888888";
        user.picture = "Camera/last_shot.jpg";
        user.password = NULL;

        Gui_userModified(user, "29416637637");
        sleep(1);
    }
    if(REMOVE_USER)
    {
        //connexion avec un badge admin et mot de passe correct
        sleep(2);
        Brain_tagReaded("29416637637");
        sleep(3);
        Gui_askConnexion("12345678");
        sleep(1);
        Gui_changeMode(MODE_ADMIN);
        sleep(1);
        Gui_userRemoved("1235");
        sleep(1);
        Gui_changeMode(MODE_CLASSIC);
        sleep(1);
    }
    if(SEARCH_USER)
    {
        //connexion avec un badge admin et mot de passe correct
        sleep(2);
        Brain_tagReaded("29416637637");
        sleep(3);
        Gui_askConnexion("12345678");
        sleep(1);
        Gui_changeMode(MODE_ADMIN);
        sleep(1);

        User* user_search;
        char* search = "val";
        user_search = Manager_searchUser(search);

        for(int i = 0; i<Archivist_getNbEmployee(search);i++){
            printf("User %d : %s\n", i+1, user_search[i].name);
            printf("User %d : %s\n", i+1, user_search[i].firstName);
            printf("User %d : %d\n", i+1, user_search[i].role);
            printf("User %d : %s\n", i+1, user_search[i].idTag);
            printf("User %d : %d\n", i+1, user_search[i].access[CURRENT_ZONE]);
            printf("User %d : %s\n", i+1, user_search[i].password);
        }

    }



    //arrêt de l'application
    Archivist_clearImages();
    Archivist_close();
    Brain_stopVisiolock();
    Rfid_stop();
    AI_stop();
    sleep(3);
    Brain_free();
    Rfid_free();
    AI_free();
    
    printf("clean\n");    
    return 0;
}
