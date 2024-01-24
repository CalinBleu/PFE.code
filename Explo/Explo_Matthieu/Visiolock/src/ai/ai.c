/*
 * INCLUDES
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <mqueue.h>
#include "ai.h"
#include "guard/guard.h"
#include "../common.h"

/*
 * DEFINES
 */
#define NAME_MQ_BOX_AI  "/mq_AI" //Boîte aux lettres liée à AI
#define MQ_MSG_COUNT 10

/*
 * TYPEDEF
 */
typedef enum  {S_FORGET = 0, S_STANDBY, S_ANALYSE, S_DEATH, STATE_NB} State;  //Etats d'IA
typedef enum  {E_START_RECO = 0, E_STOP_RECO, E_SUCCESSFUL_ANALYSIS, E_FACE_UNKNOWN, E_STOP, EVENT_NB} Event; //Evenements d'IA 
typedef enum  {A_NOP = 0, A_START_ANALYSE, A_RESULT_UNKNOWN, A_RESULT_ALLOWED, A_STOP_ANALYSE, A_STOP} Action;  //Actions réalisées par IA

/**
 * brief Structure des transitions liées à IA. Etat de destination et action à réaliser.
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
    char *picturePath;
    pid_t pid; // Champs pour stocker le PID du processus du script python
    bool analysing;
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
static void AI_mqReceive(MqMsg * aMsg);

static void AI_mqSend(MqMsg * aMsg);

static void AI_performAction(Action anAction, MqMsg * aMsg);

static void *AI_analyse(void *picturePath);

static void *AI_run(void * aParam);

static pthread_t AI_thread; 
static pthread_t analyse_thread; 

static mqd_t AI_mq;

/**
 * brief Tableau des transitions de la machine à états d'RFID
 * 
 */
static Transition mySm [STATE_NB-1][EVENT_NB] = //Transitions état-action selon l'état courant et l'évènement reçu
{
    [S_STANDBY][E_START_RECO] = {S_ANALYSE, A_START_ANALYSE},
    [S_ANALYSE][E_SUCCESSFUL_ANALYSIS] = {S_STANDBY, A_RESULT_ALLOWED},
    [S_STANDBY][E_FACE_UNKNOWN] = {S_STANDBY, A_RESULT_UNKNOWN},
    [S_ANALYSE][E_FACE_UNKNOWN] = {S_STANDBY, A_RESULT_UNKNOWN},
    [S_ANALYSE][E_STOP_RECO] = {S_STANDBY, A_STOP_ANALYSE},

    [S_STANDBY][E_STOP] = {S_DEATH, A_STOP},    
    [S_ANALYSE][E_STOP] = {S_DEATH, A_STOP}
};

/**
 * brief Fonction permettant d'initialiser la boite aux lettres
 * 
 */
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

/**
 * brief Fonction permettant de lancer le thread RFID
 */
int AI_start(void)
{
	if(pthread_create(&AI_thread, NULL, AI_run, NULL) != 0)
	{
		fprintf(stderr, "pthread_create AI error\n");
		fflush(stderr);
		return 1;
	}
	return 0;
}

/**
 * brief Fonction permettant de libérer la mémoire et la boite aux lettres
 * 
 */
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

/**
 * brief Fonction permettant de stopper la machine à états d'IA
 * 
 */
int AI_stop(void)
{
    MqMsg msg;
    msg.data.event = E_STOP;
    AI_mqSend(&msg);
    return 0;
}

/**
 * brief Fonction d'envoi d'un message à la machine à états d'IA pour lancer la reconnaissance
 * 
 * param picturePath Chemin de l'image à analyser
 */
void AI_startRecognition(char *picturePath)
{
    MqMsg msg;
    msg.data.event = E_START_RECO;
    msg.data.picturePath = picturePath;
    AI_mqSend(&msg);
}

/**
 * brief Fonction d'envoi d'un message à la machine à états d'IA pour stopper la reconnaissance
 * 
 */
void AI_stopRecognition(void)
{
    MqMsg msg;
    msg.data.event = E_STOP_RECO;
    AI_mqSend(&msg);
}

/**
 * brief Fonction permettant de recevoir un message de la boîte aux lettres
 * 
 * param aMsg Le message reçu
 */
static void AI_mqReceive(MqMsg * aMsg)
{
	int check;
	check = mq_receive(AI_mq, aMsg->buffer, sizeof(MqMsg), NULL); //réception d'un message de la mq
    if (check != sizeof(MqMsg))
    {
        perror("Error receiving message via AI mqueue");
    }
}

/**
 * brief Fonction permettant d'envoyer un message à la boîte aux lettres
 * 
 * param aMsg Le message à envoyer
 */
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

/**
 * brief Fonction de traitement des actions liées à IA
 * 
 * param anAction L'action courante à traiter
 * param aMsg Le message associé
 */
static void AI_performAction(Action anAction, MqMsg * aMsg)
{
    switch (anAction)
    {
        case A_NOP: 
            break;
        case A_START_ANALYSE:
            if(TARGET)
            {
                if(pthread_create(&analyse_thread, NULL, AI_analyse, (void *)aMsg->data.picturePath) != 0) //création du thread run d'RFID
                {
                    fprintf(stderr, "pthread_create AI error\n");
                    fflush(stderr);
                }
            }
            printf("%s : A_START_ANALYSE\n", __FILE__);
            break;
        case A_RESULT_UNKNOWN:
            Guard_resultRecognition(FACE_UNKNOWN);
            printf("%s : A_RESULT_UNKNOWN\n", __FILE__);
            break;
        case A_RESULT_ALLOWED:
            Guard_resultRecognition(ALLOWED);
            printf("%s : A_RESULT_ALLOWED\n", __FILE__);
            break;
        case A_STOP_ANALYSE:
            if(TARGET)
            {
                pthread_cancel(analyse_thread);
                pthread_join(analyse_thread, NULL);
            }
            printf("%s : A_STOP_ANALYSE\n", __FILE__);
            MqMsg msg;
            msg.data.event = E_FACE_UNKNOWN;
            AI_mqSend(&msg);
            break;
        case A_STOP:
            if(TARGET)
            {
                pthread_cancel(analyse_thread);
                pthread_join(analyse_thread, NULL);
            }
            printf("%s : A_STOP\n", __FILE__);
            break;

    }
}

/**
 * brief Fonction permettant la gestion de la machine à états d'RFID
 * 
 * param aParam Paramètre du thread
 */
static void *AI_run(void *aParam)
{
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

/**
 * brief Fonction d'analyse d'une image avec l'IA
 * 
 * param picturePath Chemin de l'image à analyser
 */
static void *AI_analyse(void *picturePath)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
    
    char result[2];
    AuthResult resultRecognition = FACE_UNKNOWN;
    int pid;

    char command[200];
    sprintf(command, "/home/facesecure/venv-python/bin/python3 face_reco.py %s %s", (char *)picturePath, CAMERA_IMAGES_PATH);
    
    while(resultRecognition != ALLOWED) {
        FILE *fp;
        fp = popen(command, "r"); // Exécuter le script Python en lecture

        if (fp == NULL) {
            perror("Erreur lors de l'exécution du script Python");
            return (void *)EXIT_FAILURE;
        }

        char pid_child[8];
        fgets(pid_child, 8, fp);

        pid = atoi(pid_child);

        fgets(result, 2, fp); // Lire la sortie du script Python dans le buffer

        pclose(fp); // Fermer le flux
        fp = NULL;

        resultRecognition = atoi(result);

        printf("Valeur renvoyée par le script Python : %d\n", (uint8_t)resultRecognition); // Afficher la valeur renvoyée
    }

    MqMsg msg;
    msg.data.pid = pid;

    if(resultRecognition == ALLOWED) {
        msg.data.event = E_SUCCESSFUL_ANALYSIS;
    }
    else if (resultRecognition == FACE_UNKNOWN){
        msg.data.event = E_FACE_UNKNOWN;
    }
    else {
        printf("Error : Result undefined %d\n", (uint8_t)resultRecognition);
    }
    AI_mqSend(&msg);

    return EXIT_SUCCESS;
}
