/*
 * INCLUDES
 */
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
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
#include <math.h>

//#include "common.h"
//#include "brain.h"
#include "MFRC522_wrapper.h"
#include "rfid.h"
/*
 * DEFINES
 */

#define NAME_MQ_BOX_RFID    "/mq_rfid" //Boîte aux lettres liée à RFID
#define MQ_MSG_COUNT        10

typedef enum  {S_FORGET = 0, S_STANDBY, S_WAITING_FOR_TAG, S_DEATH, STATE_NB} State; //Etats d'RFID
typedef enum  {E_START_READING = 0, E_SHOW_TAG, E_STOP_READING , E_STOP, EVENT_NB} Event; //Evenements d'RFID
typedef enum  {A_NOP = 0, A_START_READING, A_SHOW_TAG, A_STOP_READING ,A_STOP} Action ; //Actions réalisées par RFID

//MFRC522 mfrc;//(SS_PIN, RST_PIN);
MFRC522* mfrc;
uint64_t finalid = 0;

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

static void Rfid_mqReceive(MqMsg * aMsg);

static void Rfid_mqSend(MqMsg * aMsg);

static void Rfid_performAction(Action anAction, MqMsg * aMsg);

static void * Rfid_run(void * aParam);

static void Rfid_open();

static pthread_t rfid_thread;
static mqd_t rfid_mq; //Boîte aux lettres d'RFID
FILE *fp;

static Transition mySm [STATE_NB-1][EVENT_NB] = //Transitions état-action selon l'état courant et l'évènement reçu
{
    [S_STANDBY][E_START_READING] = {S_WAITING_FOR_TAG, A_START_READING}, 
    [S_WAITING_FOR_TAG][E_SHOW_TAG] = {S_WAITING_FOR_TAG, A_SHOW_TAG},
    [S_WAITING_FOR_TAG][E_STOP_READING] = {S_STANDBY, A_STOP_READING},
    [S_STANDBY][E_STOP] = {S_DEATH, A_STOP},
    [S_WAITING_FOR_TAG][E_STOP] = {S_DEATH, A_STOP}    
};

uint8_t Rfid_new(void)
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
    rfid_mq = mq_open(NAME_MQ_BOX_RFID, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, &mqa); //ouverture de la bal
    if(rfid_mq == -1)
    {
		perror("mq_open RFID error\n");
		return 1;
    }

    return 0;
}

uint8_t Rfid_start(void)
{
    if(pthread_create(&rfid_thread, NULL, Rfid_run, NULL) != 0) //création du thread run d'RFID
    {
        fprintf(stderr, "pthread_create RFID error\n");
        fflush(stderr);
        return 1;
    }
   
    return 0;
}

uint8_t Rfid_free(void)
{
    int check;
    check = mq_close(rfid_mq); //fermeture de la boîte aux lettres
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

static void * Rfid_run(void * aParam)
{
    printf("RFID run\n");
	MqMsg msg;
    State myState = S_STANDBY;
    Transition * myTrans;
    
	while (myState != S_DEATH)
    {
        Rfid_mqReceive(&msg);
        myTrans = &mySm[myState][msg.data.event];
        //printf("%s : %d : myState : %d\n", __FILE__, __LINE__, myState);
        //printf("%s : %d : event : %d\n", __FILE__, __LINE__, msg.data.event);
        //printf("%s : %d : dest state : %d\n", __FILE__, __LINE__, myTrans->destinationState);
        //printf("%s : %d : action : %d\n", __FILE__, __LINE__, myTrans->action);
        if (myTrans->destinationState != S_FORGET)
        {
            Rfid_performAction(myTrans->action, &msg);
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
static void Rfid_mqSend(MqMsg * aMsg)
{
    int check;
    check = mq_send(rfid_mq, aMsg->buffer, sizeof(MqMsg), 0); //envoi d'un message à la mq
    if (check == -1) {
        perror("Error sending message via RFID mqueue");
    }
}

/**
 * brief Fonction de réception des messages sur la boîte aux lettres
 * 
 * param aMsg Le message à lire
 */
static void Rfid_mqReceive(MqMsg * aMsg)
{
	int check;
	check = mq_receive(rfid_mq, aMsg->buffer, sizeof(MqMsg), NULL); //réception d'un message de la mq
    if (check != sizeof(MqMsg))
    {
        perror("Error receiving message via RFID mqueue");
    }
}


/**
 * brief Fonction d'ouverture du processus python de lecture du tag RFID
 */
static void Rfid_open(){
    if(!MRFC522_fct_PICC_IsNewCardPresent(&mfrc)) //mfrc.PICC_IsNewCardPresent())
        Rfid_showTag();
}

/**
 * brief Fonction de traitement des actions liées à RFID
 * 
 * param anAction L'action courante à traiter
 * param aMsg Le message associé
 */
static void Rfid_performAction(Action anAction, MqMsg * aMsg)
{
    switch (anAction)
    {
        case A_NOP: 
            break;
        case A_START_READING:
            Rfid_open();
            printf("%s : A_START_READING\n", __FILE__);
            break;
        case A_SHOW_TAG: 
            finalid = 0;
            if(!MRFC522_fct_PICC_ReadCardSerial(&mfrc)){//mfrc.PICC_ReadCardSerial()) {
                for (uint8_t i = 0; i < MFRC522_uid_size(&mfrc)/*mfrc.uid.size*/; ++i) {
                    if (MFRC522_uid_uidByte(&mfrc, i)/*mfrc.uid.uidByte[i]*/ < 0x10) {
                        printf("0%X", MFRC522_uid_uidByte(&mfrc, i)/*mfrc.uid.uidByte[i]*/);
                    } else {
                        printf("%X", MFRC522_uid_uidByte(&mfrc, i)/*mfrc.uid.uidByte[i]*/);
                    }
                    finalid += MFRC522_uid_uidByte(&mfrc, i)/*mfrc.uid.uidByte[i]*/ * pow(256, MFRC522_uid_size(&mfrc)/*mfrc.uid.size*/-1-i);
                }
                printf(" Final id : %010llu", finalid);
                printf("\n");
            }
            sleep(1);
            break;
        case A_STOP_READING:
            #if TARGET
            pclose(fp);
            #endif
            printf("%s : A_STOP_READING\n", __FILE__);
            break;
        case A_STOP: //signale au thread principal l'arrêt d'RFID
            #if TARGET
            pclose(fp);
            #endif
            printf("%s : A_STOP\n", __FILE__);
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
void Rfid_startReading(){
    MqMsg msg = {.data.event = E_START_READING}; //envoi de l'évènement E_START_READING via mq
	Rfid_mqSend(&msg);
}


/**
 * brief Fonction permettant de stopper la lecture après le passage d'un badge RFID
 * 
 */
void Rfid_stopReading(){
    MqMsg msg = {.data.event = E_STOP_READING}; //envoi de l'évènement E_STOP_READING via mq
	Rfid_mqSend(&msg);
}

/**
 * brief Fonction permettant de stopper la machine à états d'RFID
 * 
 */
void Rfid_stop(){
    MqMsg msg = {.data.event = E_STOP}; //envoi de l'évènement E_STOP via mq
	Rfid_mqSend(&msg);
}

/**
 * brief Fonction permettant de lire un tag présenté au lecteur
 * 
 */
void Rfid_showTag(){
    MqMsg msg = {.data.event = E_SHOW_TAG}; //envoi de l'évènement E_SHOW_TAG via mq
	Rfid_mqSend(&msg);
}


int main() {
    
    int current_uid = getuid();
    printf("My UID is: %d. My GID is: %d\n", getuid(), getgid());
    if(setuid(0)){
        printf("My UID is: %d. My GID is: %d\n", getuid(), getgid());
        perror("setuid");
        return 1;
    }
    mfrc = newMRFC522();
    //mfrc.PCD_Init();
    MFRC522_fct_PCD_Init(&mfrc);

    Rfid_new();
    Rfid_start();

    Rfid_startReading();
    

    while(1){
        finalid = 0;
        Rfid_showTag();
    }

    setuid(current_uid);
    

    //rfid_stopReading();
    //rfid_stop();
    //rfid_free();
    return 0;
}



/* READ ME :

Pour compiler 
gcc -c rfid.c -o rfid.o
g++ -c MFRC522_wrapper.cpp -o MFRC522_wrapper.o
g++ -c MFRC522.cpp -o MFRC522.o

sudo chown root.root RFID
sudo chmod 4755 RFID
./RFID

*/