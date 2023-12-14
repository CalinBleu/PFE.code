#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include "common.h"
#include "brain.h"

#define NAME_MQ_BOX_BRAIN  "/mq_brain" //Boîte aux lettres liée à Brain
#define MQ_MSG_COUNT 10

typedef enum  {S_FORGET = 0, S_INIT, S_ANALYSE, S_REGISTER, S_FACE_RECO, S_USER_DENIED, S_USER_UNKNOWN, S_OPEN_LOCK, S_IDLE, S_OFF, S_CHOICE_1, S_CHOICE_2, S_DEATH, STATE_NB} State; 
typedef enum  {E_TAG_READED = 0, E_MODE_ADMIN, E_MODE_CLASSIC, E_CHANGE_MODE_CLASSIC, E_CHANGE_MODE_ADMIN, E_USER_TAG_OK, E_ADMIN_TAG, E_USER_TAG_DENIED, E_USER_TAG_UNKNOWN, E_FACE_ANALYSED, E_FACE_TRUE, E_FACE_FALSE, E_TIMEOUT_LOCK, E_TIMEOUT_SCREEN, E_STANDBY, E_STOP, EVENT_NB} Event; 
typedef enum  {A_NOP = 0, A_TAG_READED, A_MODE_CLASSIC, A_MODE_ADMIN, A_CHANGE_MODE, A_CHANGE_MODE_SPE, A_USER_TAG_OK, A_ADMIN_TAG, A_USER_TAG_DENIED, A_USER_TAG_UNKNOWN, A_FACE_ANALYSED, A_FACE_TRUE, A_FACE_FALSE, A_TIMEOUT_LOCK, A_TIMEOUT_SCREEN, A_STANDBY, A_STOP} Action ; 

typedef struct
{
	State destinationState;
	Action action;
} Transition;

typedef struct
{
	Event event;
} MqMsgData;

typedef union
{
	MqMsgData data;
    char buffer[sizeof(MqMsgData)];
} MqMsg;

static void Brain_mqReceive(MqMsg * aMsg);

static void Brain_mqSend(MqMsg * aMsg);

static void Brain_performAction(Action anAction, MqMsg * aMsg);

static void * Brain_run(void * aParam);

static void Brain_wakeUp();

static pthread_t brain_thread; 
static mqd_t brain_mq;

static Mode mode;
static AuthResult tagResult;

static Transition mySm [STATE_NB-1][EVENT_NB] = //Transitions état-action selon l'état courant et l'évènement reçu
{
    [S_INIT][E_TAG_READED] = {S_CHOICE_1, A_TAG_READED},
    [S_CHOICE_1][E_MODE_CLASSIC] = {S_ANALYSE, A_MODE_CLASSIC},
    [S_CHOICE_1][E_MODE_ADMIN] = {S_REGISTER, A_MODE_ADMIN},
    [S_REGISTER][E_CHANGE_MODE_ADMIN] = {S_REGISTER, A_CHANGE_MODE},
    [S_REGISTER][E_CHANGE_MODE_CLASSIC] = {S_REGISTER, A_CHANGE_MODE},
    [S_ANALYSE][E_USER_TAG_OK] = {S_FACE_RECO, A_USER_TAG_OK},
    [S_ANALYSE][E_ADMIN_TAG] = {S_FACE_RECO, A_ADMIN_TAG},
    [S_ANALYSE][E_USER_TAG_UNKNOWN] = {S_USER_UNKNOWN, A_USER_TAG_UNKNOWN},
    [S_ANALYSE][E_USER_TAG_DENIED] = {S_USER_DENIED, A_USER_TAG_DENIED},
    [S_FACE_RECO][E_CHANGE_MODE_CLASSIC] = {S_FACE_RECO, A_CHANGE_MODE},
    [S_FACE_RECO][E_CHANGE_MODE_ADMIN] = {S_IDLE, A_CHANGE_MODE_SPE},
    [S_FACE_RECO][E_FACE_ANALYSED] = {S_CHOICE_2, A_FACE_ANALYSED},
    [S_CHOICE_2][E_FACE_TRUE] = {S_OPEN_LOCK, A_FACE_TRUE},
    [S_CHOICE_2][E_FACE_FALSE] = {S_USER_UNKNOWN, A_FACE_FALSE},
    [S_OPEN_LOCK][E_TIMEOUT_LOCK] = {S_IDLE, A_TIMEOUT_LOCK},
    [S_USER_UNKNOWN][E_TIMEOUT_SCREEN] = {S_IDLE, A_TIMEOUT_SCREEN},
    [S_USER_DENIED][E_TIMEOUT_SCREEN] = {S_IDLE, A_TIMEOUT_SCREEN},
    [S_REGISTER][E_TAG_READED] = {S_CHOICE_1, A_TAG_READED},
    [S_FACE_RECO][E_TAG_READED] = {S_CHOICE_1, A_TAG_READED},
    [S_IDLE][E_TAG_READED] = {S_CHOICE_1, A_TAG_READED},
    //standBy()
    [S_ANALYSE][E_STANDBY] = {S_OFF, A_STANDBY},
    [S_REGISTER][E_STANDBY] = {S_OFF, A_STANDBY},
    [S_FACE_RECO][E_STANDBY] = {S_OFF, A_STANDBY},
    [S_OPEN_LOCK][E_STANDBY] = {S_OFF, A_STANDBY},
    [S_USER_DENIED][E_STANDBY] = {S_OFF, A_STANDBY},
    [S_USER_UNKNOWN][E_STANDBY] = {S_OFF, A_STANDBY},
    [S_IDLE][E_STANDBY] = {S_OFF, A_STANDBY},
    [S_OFF][E_TAG_READED] = {S_CHOICE_1, A_TAG_READED},
    //stopVisiolock()
    [S_ANALYSE][E_STOP] = {S_DEATH, A_STOP},
    [S_REGISTER][E_STOP] = {S_DEATH, A_STOP},
    [S_FACE_RECO][E_STOP] = {S_DEATH, A_STOP},
    [S_OPEN_LOCK][E_STOP] = {S_DEATH, A_STOP},
    [S_USER_DENIED][E_STOP] = {S_DEATH, A_STOP},
    [S_USER_UNKNOWN][E_STOP] = {S_DEATH, A_STOP},
    [S_IDLE][E_STOP] = {S_DEATH, A_STOP}
};

int Brain_new(void)
{
    int check;
    struct mq_attr mqa = {
        .mq_maxmsg = MQ_MSG_COUNT,
        .mq_msgsize = sizeof(MqMsg),
        .mq_flags = 0,
    }; //Attributs de la bal
    
    check = mq_unlink(NAME_MQ_BOX_BRAIN);
    if((check == -1) && (errno =! ENOENT))
    {
        printf("mq_unlink Brain error\n");
    }
    brain_mq = mq_open(NAME_MQ_BOX_BRAIN, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, &mqa); //ouverture de la bal
    if(brain_mq == -1)
    {
		perror("mq_open Brain error\n");
		return 1;
    }

    return 0;
}

int Brain_start(void)
{
    if(pthread_create(&brain_thread, NULL, Brain_run, NULL) != 0)
    {
        fprintf(stderr, "pthread_create Brain error\n");
        fflush(stderr);
        return 1;
    }
    printf("thread created\n");
    return 0;
}

void Brain_stopVisiolock(void)
{
	MqMsg msg = {.data.event = E_STOP}; //envoi de l'évènement STOP via mq
	Brain_mqSend(&msg);
}

int Brain_free(void)
{
    int check;
    check = mq_close(brain_mq); //fermeture de la boîte aux lettres
    if(check == -1)
    {
		perror("mq_close Brain error\n");
		return 1;
    }
    check = mq_unlink(NAME_MQ_BOX_BRAIN); //suppression de la boîte aux lettres
    if(check == -1)
    {
		perror("mq_unlink Brain error\n");
		return 1;
    }
    return 0;
}

static void Brain_mqReceive(MqMsg * aMsg)
{
	int check;
	check = mq_receive(brain_mq, aMsg->buffer, sizeof(MqMsg), NULL); //réception d'un message de la mq
    if (check != sizeof(MqMsg))
    {
        perror("Error receiving message via brain mqueue");
    }
}

static void Brain_mqSend(MqMsg * aMsg)
{
    int check;
    check = mq_send(brain_mq, aMsg->buffer, sizeof(MqMsg), 0); //envoi d'un message à la mq
    if (check == -1) {
        perror("Error sending message via brain mqueue");
    }
    else
    {
        printf("Message sent via brain mq\n");
    }
}

static void Brain_performAction(Action anAction, MqMsg * aMsg)
{
    switch (anAction)
    {
        case A_NOP: 
            break;
        case A_TAG_READED:
            Brain_wakeUp();
            //GUI_screenOn();
            //RFID_stopReading();
            printf("A_TAG_READED\n");
            break;
        case A_MODE_CLASSIC:
            //tagResult = Guard_checkTag(idTag);
            printf("A_MODE_CLASSIC\n");
            break;
        case A_MODE_ADMIN:
            //GUI_setTag(idTag);
            //RFID_startReading();
            printf("A_MODE_ADMIN\n");
            break;
        case A_CHANGE_MODE:
            //mode = new_mode;
            printf("A_CHANGE_MODE\n");
            break;
        case A_CHANGE_MODE_SPE:
            //mode = new_mode;
            //RFID_startReading();
            printf("A_CHANGE_MODE_SPE\n");
            break;
        case A_USER_TAG_OK:
            //GUI_displayHomeScreen(USER_TAG_OK);
            //Guard_checkFace(idTag);
            printf("A_USER_TAG_OK\n");
            break;
        case A_USER_TAG_DENIED:
            //GUI_displayHomeScreen(USER_TAG_DENIED);
            //Doorman_userDenied();
            printf("A_USER_TAG_DENIED\n");
            break;
        case A_USER_TAG_UNKNOWN:
            //GUI_displayHomeScreen(USER_TAG_UNKNOWN);
            //Doorman_userUnknown();
            printf("A_USER_TAG_UNKNOWN\n");
            break;
        case A_FACE_ANALYSED:
            //Brain_evaluateState()
        default:
            printf("Action inconnue\n");
            break;
    }
}

static void * Brain_run(void * aParam)
{
    printf("run\n");
	MqMsg msg;
    State myState = S_INIT;
    Transition * myTrans;
	while (myState != S_DEATH)
    {
        Brain_mqReceive(&msg);
        myTrans = &mySm[myState][msg.data.event];
        printf("action : %d\n", myTrans->action);
        if (myTrans->destinationState != S_FORGET)
        {
            Brain_performAction(myTrans->action, &msg);
            myState = myTrans->destinationState;
        }
        else
        {
            printf("Brain : Event %d ignored\n", msg.data.event);
        }
    }
    printf("end of thread Brain\n");
    return NULL;
}

int main(){
    if(Brain_new() != 0) 
    {
        perror("Brain_launch error\n");
        exit(EXIT_FAILURE);
    }
    if(Brain_start() != 0)
    {
        perror("Brain_launch error\n");
        exit(EXIT_FAILURE);
    }

    sleep(10);

    Brain_free();
}