/*
 * INCLUDES
 */

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <mqueue.h>
#include "postman.h"
#include "dispatcher.h"
#include "protocol.h"
#include "common.h"

/*
 * DEFINES
 */
 
#define NAME_MQ_POSTMAN  "/mq_postman" 
#define MQ_MSG_COUNT 10 

/*
 * TYPEDEF
 */

struct sockaddr_in my_address, client_address;

typedef enum  {S_FORGET = 0, S_ON, S_DEATH, STATE_NB} State; //Etats du facteur 
typedef enum  {E_SEND, E_STOP, EVENT_NB} Event; //Evenements du facteur : demandes de lecture, envoi et arrêt
typedef enum  {A_NOP = 0, A_SEND, A_STOP} Action ; // Action réalisées par le facteur : read et write sur la socket

/**
 * @brief Structure des transitions liées au facteur. Etat de destination et action à réaliser
 * 
 */
typedef struct
{
	State destinationState;
	Action action;
} Transition;

/**
 * @brief Structure des données d'un message de la boîte aux lettres. Evènement à traiter et éventuelle donnée, ici une trame à envoyer ou lire
 * 
 */
typedef struct
{
	Event event;
    char frame[MAX_TCP_LENGTH];
    uint16_t frameSize;
} MqMsgData;

/**
 * @brief Structure d'un message de la boîte aux lettres. Donnée définie dans la structure MqMsgData et son buffer associé
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
static void * postman_run();
static void postman_mqReceive(MqMsg * aMsg);
static void postman_mqSend(MqMsg * aMsg);
static void postman_performAction(Action anAction, MqMsg * aMsg);
static void * postman_setUpConnection();

 /*
 * LOCAL VARIABLES
 */
static pthread_t postman_thread; //Thread associé au facteur
static pthread_t connection_thread;
static mqd_t postman_mq; //Boîte aux lettres du facteur

static int socketVisiolock; //Socket d'écoute 
static int socketGUI; //Socket de donnée

static Transition mySm [STATE_NB-1][EVENT_NB] =
{
    [S_ON][E_SEND]={S_ON, A_SEND},
    [S_ON][E_STOP]= {S_DEATH, A_STOP},
};

/*******************************************************************************************/
uint8_t postman_start()
{
    if(pthread_create(&connection_thread, NULL, postman_setUpConnection, NULL) != 0)
    {
        perror("pthread_create postman connection error\n");
		return 1;
    }
    else
    {
        pthread_detach(connection_thread);
    }
    if(pthread_create(&postman_thread, NULL, postman_run, NULL) != 0)
    {
		perror("pthread_create postman read error\n");
		return 1;
    }
    else
    {
        pthread_detach(postman_thread);
    }

    dispatcher_start();

    return 0;
}

uint8_t postman_new()
{
    int check;
    struct mq_attr mqa = {
        .mq_maxmsg = MQ_MSG_COUNT,
        .mq_msgsize = sizeof(MqMsg),
        .mq_flags = 0,
    };
    check = mq_unlink(NAME_MQ_POSTMAN);
    if((check == -1) && (errno == ENOENT))
    {
        printf("mq doesn't exist : %d\n", check);
    }
	postman_mq = mq_open(NAME_MQ_POSTMAN, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, &mqa);
    if(postman_mq == -1)
    {
		perror("mq_open postman error\n");
		return 1;
    }

    if(dispatcher_new() != 0)
    {
        postman_free();
        return 1;
    }
    return 0;
}

uint8_t postman_free()
{
	int check;
    check = mq_close(postman_mq);
    if(check == -1)
    {
		perror("mq_close postman error\n");
		return 1;
    }
	check = mq_unlink(NAME_MQ_POSTMAN);
    if(check == -1)
    {
		perror("mq_unlink postman error\n");
		return 1;
    }
    return 0;
}

void postman_stop(void)
{
    MqMsg msg = {.data.event = E_STOP};
    postman_mqSend(&msg);
    dispatcher_stop();
}

char* postman_readMessage(int size) 
{
    char* msg = malloc(size);
    int bytesRead = 0;
    int bytesToRead = size;

    while (bytesRead < bytesToRead) {
        int result = read(socketGUI, msg + bytesRead, bytesToRead - bytesRead);

        if (result < 0) {
            printf("Error occurred while reading socket with size %d\n", size);
            free(msg);
            msg = NULL;
            break;
        } else {
            bytesRead += result;
        }
    }
    return msg;
}


void postman_askSendMessage(char* frame)
{
    MqMsg msg;
    msg.data.event = E_SEND;
    msg.data.frameSize = protocol_getIntLength(frame[1], frame[0]);
    memcpy(msg.data.frame, frame, msg.data.frameSize);

    postman_mqSend(&msg);
    protocol_hexdump(msg.data.frame, msg.data.frameSize);
}

/**
 * @brief Fonction d'initialisation du serveur
 * 
 */
static void * postman_setUpConnection()
{
    socketVisiolock = socket(AF_INET, SOCK_STREAM, 0); //initialisation de la socket d'écoute
    memset(&my_address, 0, sizeof(my_address));
    my_address.sin_family = AF_INET;
    my_address.sin_port = htons(SERVER_PORT);
    my_address.sin_addr.s_addr = INADDR_ANY;

    bind(socketVisiolock, (struct sockaddr *)&my_address, sizeof(my_address)); //liaison de la socket à l'adresse du serveur
    listen(socketVisiolock, MAX_PENDING_CONNECTIONS); //passage en mode attente de connexion
    socklen_t clientAddressLength = sizeof(client_address);
    printf("wait accept\n");
    socketGUI = accept(socketVisiolock, (struct sockaddr *)&client_address, &clientAddressLength);
    if (socketGUI < 0) {
        perror("Erreur lors de la création du socket client");
        exit(EXIT_FAILURE);
    }
    if (socketGUI>0)
    {
        printf("connection established\n");
        dispatcher_setConnected(true);
    }

    return NULL;
}

/**
 * @brief Fonction de réception des messages sur la boîte aux lettres
 * 
 * @param aMsg Le message à lire
 */
static void postman_mqReceive(MqMsg * aMsg)
{
	int check;
	check = mq_receive(postman_mq, aMsg->buffer, sizeof(MqMsg), NULL);
    if (check != sizeof(MqMsg)) {
        perror("Error receiving message via postman mqueue");
    }
    else
    {
        printf("Message received from postman mq\n");
    }
}

/**
 * @brief Fonction d'envoi des messages sur la boîte aux lettres
 * 
 * @param aMsg Le message à envoyer
 */
static void postman_mqSend(MqMsg * aMsg)
{
	int check;
    check = mq_send(postman_mq, aMsg->buffer, sizeof(MqMsg), 0);
    if (check == -1) {
        perror("Error sending message via postman mqueue");
    }
    else
    {
        printf("Message sent via postman mq\n");
    }
}

/**
 * @brief Fonction de traitement des actions liées au facteur
 * 
 * @param anAction L'action courante à traiter
 * @param aMsg Le message associé, utilisé s'il contient des trames
 */
static void postman_performAction(Action anAction, MqMsg * aMsg)
{
	switch (anAction)
    {
    case A_NOP: 
        break;
    case A_SEND: ;
        ssize_t bytes_sent = write(socketGUI, &aMsg->data.frame, aMsg->data.frameSize);
        while(bytes_sent < aMsg->data.frameSize)
        {
            bytes_sent+= write(socketGUI, &aMsg->data.frame, aMsg->data.frameSize);
        }
        printf("MESSAGE SENT : %d\n", bytes_sent);
        break;
    case A_STOP:
        close(socketVisiolock);
        break;
    default:
        printf("Action inconnue\n");
        break;
    }
}

/**
 * @brief Fonction d'arrière plan du thread lié au facteur; Reçoie les message de la boîte aux lettres
 * 
 * @param aParam Paramètre non utilisé
 * @return void* 
 */
static void * postman_run()
{
	MqMsg msg;
    State myState = S_ON;
    Transition * myTrans;
	while (myState != S_DEATH)
    {
        postman_mqReceive(&msg);
        myTrans = &mySm[myState][msg.data.event];
        //TRACE("myState : %d\n", myState);
        //TRACE("event : %d\n", msg.data.event);
        //TRACE("dest state : %d\n", myTrans->destinationState);
        printf("action : %d\n", myTrans->action);
        if (myTrans->destinationState != S_FORGET)
        {
            postman_performAction(myTrans->action, &msg);
            myState = myTrans->destinationState;
        }
        else
        {
            printf("Postman : Event %d ignoré\n", msg.data.event);
        }
    }
    printf("postman stopped, wait end of thread\n");
    printf("end of thread postman\n");

    return NULL;
}

