#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <errno.h>

static char* currentTag;

#define NAME_MQ_BOX_BRAIN  "/mq_brain" //Boîte aux lettres liée à Brain
#define MQ_MSG_COUNT 10

typedef enum  {S_FORGET = 0, S_INIT, S_ANALYSE, S_REGISTER, S_FACE_RECO, S_USER_DENIED, S_USER_UNKNOWN, S_OPEN_LOCK, S_IDLE, S_OFF, S_CHOICE_1, S_CHOICE_2, S_DEATH, STATE_NB} State; 
typedef enum  {E_TAG_READED = 0, E_MODE_ADMIN, E_MODE_CLASSIC, E_CHANGE_MODE_CLASSIC, E_CHANGE_MODE_ADMIN, E_USER_TAG_OK, E_ADMIN_TAG, E_USER_TAG_DENIED, E_USER_TAG_UNKNOWN, E_FACE_ANALYSED, E_FACE_TRUE, E_FACE_FALSE, E_TIMEOUT, E_STANDBY, E_STOP, EVENT_NB} Event; 
typedef enum  {A_NOP = 0, A_TAG_READED, A_MODE_CLASSIC, A_MODE_ADMIN, A_CHANGE_MODE, A_CHANGE_MODE_SPE, A_USER_TAG_OK, A_ADMIN_TAG, A_USER_TAG_DENIED, A_USER_TAG_UNKNOWN, A_FACE_ANALYSED, A_FACE_TRUE, A_FACE_FALSE, A_TIMEOUT, A_STANDBY, A_STOP} Action ; 

typedef struct
{
	State destinationState;
	Action action;
} Transition;

typedef struct
{
	Event event;
    bool recognized;
} MqMsgData;

typedef union
{
	MqMsgData data;
    char buffer[sizeof(MqMsgData)];
} MqMsg;

int start(void);

int new(void);

void stopVisiolock(void);

static void mqReceive(MqMsg * aMsg);

static void mqSend(MqMsg * aMsg);

static void performAction(Action anAction, MqMsg * aMsg);

static void * run(void * aParam);

void tagReaded(char* idtag);

static pthread_t brain_thread; 
static mqd_t brain_mq;
static timer_t screen_lock_timer;

static Transition mySm [STATE_NB-1][EVENT_NB] = //Transitions état-action selon l'état courant et l'évènement reçu
{
    [S_INIT][E_MODE_CLASSIC] = {S_ANALYSE, A_MODE_CLASSIC},
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
    [S_OPEN_LOCK][E_TIMEOUT] = {S_IDLE, A_TIMEOUT},
    [S_USER_UNKNOWN][E_TIMEOUT] = {S_IDLE, A_TIMEOUT},
    [S_USER_DENIED][E_TIMEOUT] = {S_IDLE, A_TIMEOUT},
    [S_REGISTER][E_TAG_READED] = {S_CHOICE_1, A_TAG_READED},
    [S_FACE_RECO][E_TAG_READED] = {S_CHOICE_1, A_TAG_READED},
    [S_OPEN_LOCK][E_TAG_READED] = {S_CHOICE_1, A_TAG_READED},
    [S_USER_DENIED][E_TAG_READED] = {S_CHOICE_1, A_TAG_READED},
    [S_USER_UNKNOWN][E_TAG_READED] = {S_CHOICE_1, A_TAG_READED},
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
    [S_INIT][E_STOP] = {S_DEATH, A_STOP},
    [S_ANALYSE][E_STOP] = {S_DEATH, A_STOP},
    [S_REGISTER][E_STOP] = {S_DEATH, A_STOP},
    [S_FACE_RECO][E_STOP] = {S_DEATH, A_STOP},
    [S_OPEN_LOCK][E_STOP] = {S_DEATH, A_STOP},
    [S_USER_DENIED][E_STOP] = {S_DEATH, A_STOP},
    [S_USER_UNKNOWN][E_STOP] = {S_DEATH, A_STOP},
    [S_IDLE][E_STOP] = {S_DEATH, A_STOP},
    [S_OFF][E_STOP] = {S_DEATH, A_STOP}
};

int new(void)
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

    currentTag = "truc";

    return 0;
}

int start(void)
{
    if(pthread_create(&brain_thread, NULL, run, NULL) != 0)
    {
        fprintf(stderr, "pthread_create Brain error\n");
        fflush(stderr);
        return 1;
    }
    return 0;
}

static void mqReceive(MqMsg * aMsg)
{
	int check;
	check = mq_receive(brain_mq, aMsg->buffer, sizeof(MqMsg), NULL); //réception d'un message de la mq
    if (check != sizeof(MqMsg))
    {
        perror("Error receiving message via brain mqueue");
    }
}

static void mqSend(MqMsg * aMsg)
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

static void performAction(Action anAction, MqMsg * aMsg)
{
    switch (anAction)
    {
        case A_NOP: 
            break;
        case A_TAG_READED:
            printf("%s : A_TAG_READED\n", __FILE__);
            break;
        case A_MODE_CLASSIC: ;
            printf("perform action : %s\n", currentTag);
            printf("%s : A_MODE_CLASSIC\n", __FILE__);
            break;
        case A_STOP:
            printf("%s : A_STOP\n", __FILE__);
            break;
        default:
            printf("Action inconnue\n");
            break;
    }
}

static void * run(void * aParam)
{
    printf("Brain run\n");
	MqMsg msg;
    State myState = S_INIT;
    Transition * myTrans;
	while (myState != S_DEATH)
    {
        mqReceive(&msg);
        myTrans = &mySm[myState][msg.data.event];
        printf("%s : %d : myState : %d\n", __FILE__, __LINE__, myState);
        printf("%s : %d : event : %d\n", __FILE__, __LINE__, msg.data.event);
        printf("%s : %d : dest state : %d\n", __FILE__, __LINE__, myTrans->destinationState);
        printf("%s : %d : action : %d\n", __FILE__, __LINE__, myTrans->action);
        if (myTrans->destinationState != S_FORGET)
        {
            performAction(myTrans->action, &msg);
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

void tagReaded(char* idtag)
{
    currentTag = idtag;
    printf("tagReaded : %s\n", currentTag);
	MqMsg msg = {.data.event = E_MODE_CLASSIC}; //envoi de l'évènement TAG_READED via mq
	mqSend(&msg);
}

void stopVisiolock(void)
{
	MqMsg msg = {.data.event = E_STOP}; //envoi de l'évènement STOP via mq
	mqSend(&msg);
}

int main()
{
    new();
    start();

    printf("before : %s\n", currentTag);

    sleep(1);

    tagReaded("1234");

    sleep(1);

    stopVisiolock();

    
}