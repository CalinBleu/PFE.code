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

int main(){

    //démarrage de l'application
    Rfid_new();
    Rfid_start();
    AI_new();
    AI_start();
    Brain_startVisiolock();
    Doorman_init();
    Archivist_open();
    
    sleep(10);

    //arrêt de l'application
    Archivist_clearImages();
    Archivist_close();
    Brain_stopVisiolock();
    Rfid_stop();
    AI_stop();
    sleep(5);
    Brain_free();
    Rfid_free();
    AI_free();
    
    printf("clean\n");    
    return 0;
}
