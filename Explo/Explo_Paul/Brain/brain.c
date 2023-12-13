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

#define NAME_MQ_BOX_UISS  "/mq_brain" //Boîte aux lettres liée à Brain
#define MQ_MSG_COUNT 10

typedef enum  {S_FORGET = 0, S_INIT, S_ANALYSE, S_REGISTER, S_FACE_RECO, S_USER_DENIED, S_USER_UNKNOWN, S_OPEN_LOCK, S_IDLE, S_OFF, S_CHOICE_1, S_CHOICE_2, S_DEATH, STATE_NB} State; 
typedef enum  {E_START = 0, E_TAG_READED, E_MODE_ADMIN, E_MODE_CLASSIC, E_CHANGE_MODE_CLASSIC, E_CHANGE_MODE_ADMIN, E_USER_TAG_OK, E_ADMIN_TAG, E_USER_TAG_DENIED, E_USER_TAG_UNKNOWN, E_FACE_ANALYSED, E_FACE_TRUE, E_FACE_FALSE, E_TIMEOUT_LOCK, E_TIMEOUT_SCREEN, E_STANDBY, E_STOP, EVENT_NB} Event; 
typedef enum  {A_NOP = 0, A_START, A_TAG_READED, A_MODE_CLASSIC, A_MODE_ADMIN, A_CHANGE_MODE_CLASSIC, A_CHANGE_MODE_ADMIN, A_USER_TAG_OK, A_ADMIN_TAG, A_USER_TAG_DENIED, A_USER_TAG_UNKNOWN, A_FACE_ANALYSED, A_FACE_TRUE, A_FACE_FALSE, A_TIMEOUT_LOCK, A_TIMEOUT_SCREEN, A_STANDBY, A_STOP} Action ; 

typedef struct
{
	State destinationState;
	Action action;
} Transition;

typedef struct
{
	Event event;
    bool state;
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

static Transition mySm [STATE_NB-1][EVENT_NB] = //Transitions état-action selon l'état courant et l'évènement reçu
{
    [S_INIT][E_TAG_READED] = {S_CHOICE_1, A_TAG_READED},
    //TODO très long
};



int main(){
    return 0;
}