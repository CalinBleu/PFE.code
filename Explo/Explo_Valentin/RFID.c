/*
 * INCLUDES
 */
#include <stdio.h>
#include <unistd.h>
#include "RFID.h"
//#include "Brain.h"


FILE *fp;


/**
 * brief Fonction permettant de mettre en marche la lecture d'un badge RFID
 * 
 */
void RFID_startReading(){
    fp = popen("python3 read.py", "r");
    if (fp == NULL) {
        printf("Failed to run command\n" );
    }
    RFID_showTag();
}

/**
 * brief Fonction permettant de stopper la lecture après le passage d'un badge RFID
 * 
 */
void RFID_stopReading(){
    pclose(fp);
}

/**
 * brief Fonction permettant de lire un tag présenté au lecteur
 * 
 */
void RFID_showTag(){
    char buff[30];
    while (fgets(buff, sizeof(buff)-1, fp) != NULL) {
        printf("%s", buff);
        //Brain_tagReaded(buff);
    }
   
}


int main() {
    RFID_startReading();
    RFID_stopReading();
    
    return 0;
}
