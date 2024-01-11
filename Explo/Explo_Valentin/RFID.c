/*
 * INCLUDES
 */
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include "RFID.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <mqueue.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <signal.h>
#include <stdint.h>




//#include "Brain.h"

/*
 * DEFINES
 */

#define NAME_MQ_BOX_RFID    "/mq_rfid" //Boîte aux lettres liée à RFID
#define MQ_MSG_COUNT        10

typedef enum  {S_FORGET = 0, S_STANDBY, S_WAITING_FOR_TAG, S_DEATH, STATE_NB} State; //Etats d'RFID
typedef enum  {E_ASK_START_COM = 0, E_START_READING, E_SHOW_TAG, E_STOP_READING ,E_STOP, EVENT_NB} Event; //Evenements d'RFID : demandes d'affichage de l'état de la caméra, état de la porte et visage non reconnu
typedef enum  {A_NOP = 0, A_START_READING, A_TAG_READED, A_STOP_READING ,A_STOP} Action ; //Actions réalisées par RFID


static pthread_t RFID_thread;
static mqd_t RFID_mq; //Boîte aux lettres d'RFID
static pthread_barrier_t start_barrier;

/*
 * TYPEDEF
 */

/**
 * brief Structure des transitions liées à RFID. Etat de destination et action à réaliser.
 * 
 */
typedef struct
{
	State destinationState;
	Action action;
} Transition;

/**
 * brief Structure des données d'un message de la boîte aux lettres. Evènement à traiter et éventuelle donnée, ici un état de WebCam/porte.
 * 
 */
typedef struct
{
	Event event;
    bool state;
} MqMsgData;

/**
 * brief Structure d'un message de la boîte aux lettres. Donnée définie dans la structure MqMsgData et son buffer associé.
 * 
 */
typedef union
{
	MqMsgData data;
    char buffer[sizeof(MqMsgData)];
} MqMsg;





/*
 * LOCAL FUNCTIONS
 */

static void RFID_mqReceive(MqMsg * aMsg);

static void RFID_mqSend(MqMsg * aMsg);

static void RFID_performAction(Action anAction, MqMsg * aMsg);

static void * RFID_run(void * aParam);

static void RFID_popen();


static Transition mySm [STATE_NB-1][EVENT_NB] = //Transitions état-action selon l'état courant et l'évènement reçu
{
    [S_STANDBY][E_START_READING] = {S_WAITING_FOR_TAG, A_START_READING}, 
    [S_WAITING_FOR_TAG][E_SHOW_TAG] = {S_WAITING_FOR_TAG, A_TAG_READED},
    [S_WAITING_FOR_TAG][E_STOP_READING] = {S_STANDBY, A_STOP_READING},
    [S_STANDBY][E_STOP] = {S_DEATH, A_STOP},
    [S_WAITING_FOR_TAG][E_STOP] = {S_DEATH, A_STOP}    
};

uint8_t RFID_new(void)
{
    int check;
    struct mq_attr mqa = {
        .mq_maxmsg = MQ_MSG_COUNT,
        .mq_msgsize = sizeof(MqMsg),
        .mq_flags = 0,
    }; //Attributs de la bal
    
    check = mq_unlink(NAME_MQ_BOX_RFID);
    if((check == -1) && (errno =! ENOENT))
    {
        printf("mq_unlink RFID error\n");
    }
    RFID_mq = mq_open(NAME_MQ_BOX_RFID, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, &mqa); //ouverture de la bal
    if(RFID_mq == -1)
    {
		perror("mq_open RFID error\n");
		return 1;
    }

    pthread_barrier_init(&start_barrier, NULL, 2);
}

uint8_t RFID_start(void)
{
    if(pthread_create(&RFID_thread, NULL, RFID_run, NULL) != 0) //création du thread run d'RFID
    {
        fprintf(stderr, "pthread_create RFID error\n");
        fflush(stderr);
        return 1;
    }
   
    return 0;
}

uint8_t RFID_free(void)
{
    int check;
    check = mq_close(RFID_mq); //fermeture de la boîte aux lettres
    if(check == -1)
    {
		perror("mq_close RFID error\n");
		return 1;
    }
    check = mq_unlink(NAME_MQ_BOX_RFID); //suppression de la boîte aux lettres
    if(check == -1)
    {
		perror("mq_unlink RFID error\n");
		return 1;
    }
    return 0;
}

FILE *fp;

/*
 * GLOBAL FUNCTIONS
 */
static void * RFID_run(void * aParam)
{
	MqMsg msg;
    State myState = S_STANDBY;
    Transition * myTrans;
    
	while (myState != S_DEATH)
    {
        RFID_mqReceive(&msg);
        myTrans = &mySm[myState][msg.data.event];
        //printf("action : %d\n", myTrans->action);
        if (myTrans->destinationState != S_FORGET)
        {
            RFID_performAction(myTrans->action, &msg);
            myState = myTrans->destinationState;
        }
        else
        {
            printf("RFID : Event %d ignored\n", msg.data.event);
        }
    }
    printf("RFID stopped, wait end of thread\n");
    printf("end of thread RFID\n");
    return NULL;
}

/**
 * brief Fonction d'envoi des messages sur la boîte aux lettres
 * 
 * param aMsg Le message à envoyer
 */
static void RFID_mqSend(MqMsg * aMsg)
{
    int check;
    check = mq_send(RFID_mq, aMsg->buffer, sizeof(MqMsg), 0); //envoi d'un message à la mq
    if (check == -1) {
        perror("Error sending message via RFID mqueue");
    }
}

/**
 * brief Fonction de réception des messages sur la boîte aux lettres
 * 
 * param aMsg Le message à lire
 */
static void RFID_mqReceive(MqMsg * aMsg)
{
	int check;
	check = mq_receive(RFID_mq, aMsg->buffer, sizeof(MqMsg), NULL); //réception d'un message de la mq
    if (check != sizeof(MqMsg))
    {
        perror("Error receiving message via RFID mqueue");
    }
}


/**
 * brief Fonction d'ouverture du processus python de lecture du tag RFID
 */
static void RFID_popen(){
    fp = popen("python3 read.py", "r");
    if (fp == NULL) {
        printf("Failed to run command\n" );
    }
    MqMsg msg = {.data.event = E_SHOW_TAG}; //envoi de l'évènement E_START_READING via mq
	RFID_mqSend(&msg);
}

/**
 * brief Fonction de traitement des actions liées à RFID
 * 
 * param anAction L'action courante à traiter
 * param aMsg Le message associé
 */
static void RFID_performAction(Action anAction, MqMsg * aMsg)
{
    switch (anAction)
    {
        case A_NOP: 
            break;
        case A_START_READING:
            RFID_popen();
            break;
        case A_TAG_READED:
            RFID_showTag();
            break;
        case A_STOP_READING:
            pclose(fp);
            break;
        case A_STOP: //signale au thread principal l'arrêt d'RFID
            pclose(fp);
            printf("RFID : stop\n");
            break;
        default:
            printf("Action inconnue\n");
            break;
    }
}



/**
 * brief Fonction permettant de mettre en marche la lecture d'un badge RFID
 * 
 */
void RFID_startReading(){
    MqMsg msg = {.data.event = E_START_READING}; //envoi de l'évènement E_START_READING via mq
	RFID_mqSend(&msg);
}


/**
 * brief Fonction permettant de stopper la lecture après le passage d'un badge RFID
 * 
 */
void RFID_stopReading(){
    MqMsg msg = {.data.event = E_STOP_READING}; //envoi de l'évènement E_STOP_READING via mq
	RFID_mqSend(&msg);
}

/**
 * brief Fonction permettant de stopper la machine à états d'RFID
 * 
 */
void RFID_stop(){
    MqMsg msg = {.data.event = E_STOP}; //envoi de l'évènement E_STOP via mq
	RFID_mqSend(&msg);
}

/**
 * brief Fonction permettant de lire un tag présenté au lecteur
 * 
 */
void RFID_showTag(){
    char buff[20];
    while (fgets(buff, sizeof(buff)-1, fp) != NULL) {
        printf("%s", buff);
        //Brain_tagReaded(buff);
    }
    pthread_barrier_wait(&start_barrier);
    
}


int main() {
    RFID_new();
    RFID_start();
    RFID_startReading();
    

    pthread_barrier_wait(&start_barrier);
    RFID_stopReading();
    //RFID_stop();
    RFID_free();
    return 0;
}
