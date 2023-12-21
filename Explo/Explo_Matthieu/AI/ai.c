#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <mqueue.h>

#include "ai.h"
#include "common.h"


#define NAME_MQ_BOX_AI  "/mq_AI" //Boîte aux lettres liée à AI
#define MQ_MSG_COUNT 10

typedef enum  {S_FORGET = 0, S_STANDBY, S_ANALYSE, S_DEATH, STATE_NB} State; 
typedef enum  {E_START_RECO = 0, E_STOP_RECO, E_SUCCESSFUL_ANALYSIS, E_STOP, EVENT_NB} Event; 
typedef enum  {A_NOP = 0, A_START_ANALYSE, A_RESULT_UNKNOWN, A_RESULT_ALLOWED, A_STOP} Action; 

typedef struct
{
	State destinationState;
	Action action;
} Transition;

typedef struct
{
	Event event;
    char *picturePath;
} MqMsgData;

typedef union
{
	MqMsgData data;
    char buffer[sizeof(MqMsgData)];
} MqMsg;

static void AI_mqReceive(MqMsg * aMsg);

static void AI_mqSend(MqMsg * aMsg);

static void AI_performAction(Action anAction, MqMsg * aMsg);

static void *AI_run(void * aParam);

static pid_t pid; // Variable pour stocker le PID du processus fils

static pthread_t AI_thread; 
static mqd_t AI_mq;

static Transition mySm [STATE_NB-1][EVENT_NB] = //Transitions état-action selon l'état courant et l'évènement reçu
{
    [S_STANDBY][E_START_RECO] = {S_ANALYSE, A_START_ANALYSE},
    [S_ANALYSE][E_SUCCESSFUL_ANALYSIS] = {S_STANDBY, A_RESULT_ALLOWED},
    [S_ANALYSE][E_STOP_RECO] = {S_STANDBY, A_RESULT_UNKNOWN},

    [S_STANDBY][E_STOP] = {S_DEATH, A_STOP},    
    [S_ANALYSE][E_STOP] = {S_DEATH, A_STOP}
};


int AI_new(void)
{
    int check;
    struct mq_attr mqa = {
        .mq_maxmsg = MQ_MSG_COUNT,
        .mq_msgsize = sizeof(MqMsg),
        .mq_flags = 0,
    }; //Attributs de la bal
    
    check = mq_unlink(NAME_MQ_BOX_AI);
    if((check == -1) && (errno =! ENOENT))
    {
        printf("mq_unlink AI error\n");
    }
    AI_mq = mq_open(NAME_MQ_BOX_AI, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, &mqa); //ouverture de la bal
    if(AI_mq == -1)
    {
		perror("mq_open AI error\n");
		return 1;
    }

    return 0;
}

int AI_start(void)
{
	if(pthread_create(&AI_thread, NULL, AI_run, NULL) != 0)
	{
		fprintf(stderr, "pthread_create AI error\n");
		fflush(stderr);
		return 1;
	}
	printf("thread created\n");
	return 0;
}

int AI_free(void)
{
    int check;
    check = mq_close(AI_mq); //fermeture de la boîte aux lettres
    if(check == -1)
    {
		perror("mq_close AI error\n");
		return 1;
    }
    check = mq_unlink(NAME_MQ_BOX_AI); //suppression de la boîte aux lettres
    if(check == -1)
    {
		perror("mq_unlink AI error\n");
		return 1;
    }
    return 0;
}

static void AI_mqReceive(MqMsg * aMsg)
{
	int check;
	check = mq_receive(AI_mq, aMsg->buffer, sizeof(MqMsg), NULL); //réception d'un message de la mq
    if (check != sizeof(MqMsg))
    {
        perror("Error receiving message via AI mqueue");
    }
}

static void AI_mqSend(MqMsg * aMsg)
{
    int check;
    check = mq_send(AI_mq, aMsg->buffer, sizeof(MqMsg), 0); //envoi d'un message à la mq
    if (check == -1) {
        perror("Error sending message via AI mqueue");
    }
    else
    {
        printf("Message sent via AI mq\n");
    }
}

static void AI_performAction(Action anAction, MqMsg * aMsg)
{
    switch (anAction)
    {
        case A_NOP: 
            break;
        case A_START_ANALYSE:
            AI_startRecognition(aMsg->data.picturePath);
            printf("A_START_ANALYSE\n");
            break;
        case A_RESULT_UNKNOWN:
            //Guard_resultRecognition(FACE_UNKNOWN);
            printf("A_RESULT_UNKNOWN\n");
            break;
        case A_RESULT_ALLOWED:
            //Guard_resultRecognition(ALLOWED);
            printf("A_RESULT_ALLOWED\n");
            break;
        case A_STOP:
            printf("A_STOP\n");
            break;

    }
}

void *AI_run(void *aParam)
{
    printf("run\n");
	MqMsg msg;
    State myState = S_STANDBY;
    Transition * myTrans;
	while (myState != S_DEATH)
    {
        AI_mqReceive(&msg);
        myTrans = &mySm[myState][msg.data.event];
        if (myTrans->destinationState != S_FORGET)
        {
            AI_performAction(myTrans->action, &msg);
            myState = myTrans->destinationState;
        }
        else
        {
            printf("AI : Event %d ignored\n", msg.data.event);
        }
    }
    printf("end of thread AI\n");
    return NULL;
}

int AI_startRecognition(char *picturePath)
{
    char result[3];
    AuthResult resultRecognition = FACE_UNKNOWN;
    FILE *fp;

    char command[200];
    sprintf(command, "python3 /home/pi/Documents/PFE.code/Explo/Explo_Matthieu/IA/face_reco.py %s", picturePath);
    fp = popen(command, "r"); // Exécuter le script Python en lecture

    if (fp == NULL) {
        perror("Erreur lors de l'exécution du script Python");
        return -1;
    }

    char pid_child[6];
    fgets(pid_child, 6, fp);

    fgets(result, 3, fp); // Lire la sortie du script Python dans le buffer

    pclose(fp); // Fermer le flux

    resultRecognition = (uint8_t)(result[0] - '0');

    printf("pid : %s\n", pid_child);
    printf("Valeur renvoyée par le script Python : %d\n", (uint8_t)resultRecognition); // Afficher la valeur renvoyée

    MqMsg msg;
    msg.data.event = E_SUCCESSFUL_ANALYSIS;
    AI_mqSend(&msg);
    printf("quit start recogition");
    return 0;
}

int AI_stopRecognition(void)
{
    if (pid > 0) {
        kill(pid, SIGKILL); // Interrompre le processus fils
    }
    MqMsg msg;
    msg.data.event = E_STOP_RECO;
    AI_mqSend(&msg);
}

int main (void)
{
    AI_new();
    AI_start();
    MqMsg msg;
    msg.data.event = E_START_RECO;
    msg.data.picturePath = DATASET_PATH;
    AI_mqSend(&msg);
    sleep(20);
    msg.data.event = E_STOP;
    AI_mqSend(&msg);
    sleep(5);
    AI_free();
    return 0;
}


