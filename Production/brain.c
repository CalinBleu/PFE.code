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
#include <sys/wait.h>
#include "common.h"
#include "guard.h"
#include "brain.h"
#include "rfid.h"
#include "doorman.h"

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
    Mode mode;
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

static void Brain_evaluateState(bool state);

static void Brain_evaluateMode(Mode mode);

static void Brain_evaluateTag(AuthResult result);

static void Brain_timer_launch();

static void Brain_cancel_timer();

static void Brain_timeout(union sigval val);

static pthread_t brain_thread; 
static mqd_t brain_mq;
static timer_t screen_lock_timer;

static Mode mode;
static char currentTag[20];

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

	struct sigevent event;

	event.sigev_notify = SIGEV_THREAD;
	event.sigev_value.sival_ptr = NULL;
	event.sigev_notify_function = Brain_timeout;
	event.sigev_notify_attributes = NULL;

	if (timer_create(CLOCK_REALTIME, &event, &screen_lock_timer) != 0) {
		perror("timer_create");
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
    return 0;
}

static void Brain_timeout(union sigval val)
{
	MqMsg msg = {.data.event = E_TIMEOUT};
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
            printf("a_tag_readed : %s\n", currentTag);
            Brain_wakeUp();
            Rfid_stopReading();
            Brain_cancel_timer();
            Brain_evaluateMode(mode);
            printf("%s : A_TAG_READED\n", __FILE__);
            break;
        case A_MODE_CLASSIC: ;
            printf("a_mode_classic : %s\n", currentTag);
            AuthResult tagResult = Guard_checkTag(currentTag);
            Brain_evaluateTag(tagResult);
            printf("%s : A_MODE_CLASSIC\n", __FILE__);
            break;
        case A_MODE_ADMIN:
            //GUI_setTag(aMsg->data.idtag);
            Rfid_startReading();
            printf("%s : A_MODE_ADMIN\n", __FILE__);
            break;
        case A_CHANGE_MODE:
            mode = aMsg->data.mode;
            printf("%s : A_CHANGE_MODE\n", __FILE__);
            break;
        case A_CHANGE_MODE_SPE:
            mode = MODE_ADMIN;
            Rfid_startReading();
            printf("%s : A_CHANGE_MODE_SPE\n", __FILE__);
            break;
        case A_USER_TAG_OK: ;
            //GUI_displayHomeScreen(USER_TAG_OK);
            char* localTag2 = (char*)currentTag;
            Guard_checkFace(localTag2);
            printf("%s : A_USER_TAG_OK\n", __FILE__);
            break;
        case A_ADMIN_TAG: ;
            //GUI_displayHomeScreen(ADMIN_TAG);
            char* localTag3 = (char*)currentTag;
            Guard_checkFace(localTag3);
            printf("%s : A_ADMIN_TAG\n", __FILE__);
            break;
        case A_USER_TAG_DENIED:
            //GUI_displayHomeScreen(USER_TAG_DENIED);
            Doorman_userDenied();
            Brain_timer_launch();
            printf("%s : A_USER_TAG_DENIED\n", __FILE__);
            break;
        case A_USER_TAG_UNKNOWN:
            //GUI_displayHomeScreen(USER_TAG_UNKNOWN);
            Doorman_userUnknown();
            Brain_timer_launch();
            printf("%s : A_USER_TAG_UNKNOWN\n", __FILE__);
            break;
        case A_FACE_ANALYSED:
            Brain_evaluateState(aMsg->data.recognized);
            printf("%s : A_FACE_ANALYSED\n", __FILE__);
            break;
        case A_FACE_TRUE:
            //GUI_displayHomeScreen(ALLOWED);
            Doorman_open();
            Brain_timer_launch();
            printf("%s : A_FACE_TRUE\n", __FILE__);
            break;
        case A_FACE_FALSE:
            //GUI_displayHomeScreen(FACE_UNKNOWN);
            Doorman_userUnknown();
            Brain_timer_launch();
            printf("%s : A_FACE_FALSE\n", __FILE__);
            break;
        case A_TIMEOUT:
            Rfid_startReading();
            printf("%s : A_TIMEOUT\n", __FILE__);
            break;
        case A_STANDBY:
            //GUI_screenOff();
            Rfid_startReading();
            printf("%s : A_STANDBY\n", __FILE__);
            break;
        case A_STOP:
            Rfid_stopReading();
            //GUI_screenOff();
            printf("%s : A_STOP\n", __FILE__);
            break;
        default:
            printf("Action inconnue\n");
            break;
    }
}

static void * Brain_run(void * aParam)
{
    printf("Brain run\n");
	MqMsg msg;
    State myState = S_INIT;
    Transition * myTrans;
	while (myState != S_DEATH)
    {
        Brain_mqReceive(&msg);
        myTrans = &mySm[myState][msg.data.event];
        printf("%s : %d : myState : %d\n", __FILE__, __LINE__, myState);
        printf("%s : %d : event : %d\n", __FILE__, __LINE__, msg.data.event);
        printf("%s : %d : dest state : %d\n", __FILE__, __LINE__, myTrans->destinationState);
        printf("%s : %d : action : %d\n", __FILE__, __LINE__, myTrans->action);
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

static void Brain_evaluateState(bool state)
{
    MqMsg msg;
    if(state)
    {
        msg.data.event = E_FACE_TRUE; 
    }
    else
    {
        msg.data.event = E_FACE_FALSE; 
    }
    Brain_mqSend(&msg);
}

static void Brain_evaluateMode(Mode mode)
{
    MqMsg msg;
    if(mode == MODE_CLASSIC)
    {
        msg.data.event = E_MODE_CLASSIC; 
    }
    else
    {
        msg.data.event = E_MODE_ADMIN; 
    }
    Brain_mqSend(&msg);
}

static void Brain_evaluateTag(AuthResult result)
{
    MqMsg msg;
    if(result == USER_TAG_OK)
    {
        msg.data.event = E_USER_TAG_OK; 
    }
    else if(result == ADMIN_TAG)
    {
        msg.data.event = E_ADMIN_TAG; 
    }
    else if(result == USER_TAG_UNKNOWN)
    {
        msg.data.event = E_USER_TAG_UNKNOWN; 
    }
    else
    {
        msg.data.event = E_USER_TAG_DENIED; 
    }
    Brain_mqSend(&msg);
}

static void Brain_timer_launch()
{
	struct itimerspec itimer;

	itimer.it_interval.tv_sec = 0;
	itimer.it_interval.tv_nsec = 0;
	itimer.it_value.tv_sec = 5;
	itimer.it_value.tv_nsec = 0;

	if (timer_settime(screen_lock_timer, 0, &itimer, NULL) != 0) {
		perror("timer_settime launch");
	}
}

static void Brain_cancel_timer()
{
	struct itimerspec itimer;

	itimer.it_interval.tv_sec = 0;
	itimer.it_interval.tv_nsec = 0;
	itimer.it_value.tv_sec = 0;
	itimer.it_value.tv_nsec = 0;

	if (timer_settime(screen_lock_timer, 0, &itimer, NULL) != 0) {
		perror("timer_settime cancel");
	}
}

void Brain_startVisiolock()
{
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

    Brain_wakeUp();
    Rfid_startReading();
}

void Brain_stopVisiolock(void)
{
	MqMsg msg = {.data.event = E_STOP}; //envoi de l'évènement STOP via mq
	Brain_mqSend(&msg);
}

void Brain_standBy(void)
{
	MqMsg msg = {.data.event = E_STANDBY}; //envoi de l'évènement STANDBY via mq
	Brain_mqSend(&msg);
}

void Brain_tagReaded(char* idtag)
{
    printf("idtag param : %s\n", idtag);
    strcpy(currentTag, idtag);
    printf("tagReaded : %s\n", currentTag);
	MqMsg msg = {.data.event = E_TAG_READED}; //envoi de l'évènement TAG_READED via mq
	Brain_mqSend(&msg);
}

void Brain_changeMode(Mode mode)
{
    MqMsg msg;
    if(mode == MODE_CLASSIC)
    {
        msg.data.event = E_CHANGE_MODE_CLASSIC; 
        msg.data.mode = MODE_CLASSIC; 
    }
    else
    {
        msg.data.event = E_CHANGE_MODE_ADMIN;
        msg.data.mode = MODE_ADMIN; 
    }
    Brain_mqSend(&msg);
}

void Brain_faceAnalysed(bool recognized)
{
	MqMsg msg = {.data.event = E_FACE_ANALYSED, .data.recognized = recognized}; //envoi de l'évènement FACE_ANALYSED via mq
	Brain_mqSend(&msg);
}



static void Brain_wakeUp()
{
    //GUI_screenOn();
}