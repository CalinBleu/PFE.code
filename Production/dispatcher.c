/*
 * INCLUDES
 */

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>
#include <mqueue.h>
#include <semaphore.h>

//include objets
#include "guard.h"
#include "dispatcher.h"

//include facteur et trame
#include "postman.h"
#include "protocol.h"
#include "common.h"


/*
 * DEFINES
 */
#define NAME_MQ_DISPATCHER  "/mq_dispatcher" 
#define MQ_MSG_COUNT 10 
#define HEADER_SIZE 3

/*
 * TYPEDEF
 */
typedef enum  {S_FORGET = 0, S_ON, S_DEATH, STATE_NB} State; //Etats du dispatcher 
typedef enum  {E_READ = 0, E_DISPATCH, E_STOP, EVENT_NB} Event; //Evenements du dispatcher : demandes de lecture, envoi et arrêt
typedef enum  {A_NOP = 0, A_DISPATCH, A_STOP} Action ; // Action réalisées par le dispatcher : read et write sur la socket

/**
 * @brief Structure des transitions liées au dispatcher. Etat de destination et action à réaliser
 * 
 */
typedef struct
{
	State destinationState;
	Action action;
} Transition;

/**
 * @brief Structure des données d'un message de la boîte aux lettres. Evènement à traiter et éventuelle donnée
 * 
 */
typedef struct
{
	Event event;
    char* frame;
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

/**
 * @brief Fonction de dispatch. Joue le rôle de décodeur des trames reçues et répartit les messages selon les destinataires.
 */
static void * Dispatcher_dispatch();

/*
 * LOCAL VARIABLES
 */

static pthread_t dispatcher_thread;
static mqd_t dispatcher_mq; //Boîte aux lettres du dispatcher

static sem_t connection_sem;
static volatile bool gui_connected = false;

/*******************************************************************************************/

uint8_t Dispatcher_start()
{
    if(pthread_create(&dispatcher_thread, NULL, Dispatcher_dispatch, NULL) != 0) //création du thread du dispatcher
    {
		perror("pthread_create dispatcher error\n");
		return 1;
    }
    else
    {
        pthread_detach(dispatcher_thread);
    }
    return 0;
}

uint8_t Dispatcher_new()
{
    int check;
    struct mq_attr mqa = {
        .mq_maxmsg = MQ_MSG_COUNT,
        .mq_msgsize = sizeof(MqMsg),
        .mq_flags = 0,
    };
    check = mq_unlink(NAME_MQ_DISPATCHER);
    if((check == -1) && (errno == ENOENT))
    {
        printf("mq doesn't exist : %d\n", check);
    }
	dispatcher_mq = mq_open(NAME_MQ_DISPATCHER, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, &mqa);
    if(dispatcher_mq == -1)
    {
		perror("mq_open dispatcher error\n");
		return 1;
    }

    if(sem_init(&connection_sem, 0, 0) != 0)
    {
		perror("sem_init connection sem error\n");
		return 1;
    }
    return 0;
}

void Dispatcher_stop()
{
    Dispatcher_setConnected(false);
}

uint8_t Dispatcher_free()
{
    int check;
    check = mq_close(dispatcher_mq);
    if(check == -1)
    {
		perror("mq_close dispatcher error\n");
        return 1;
    }
	check = mq_unlink(NAME_MQ_DISPATCHER);
    if(check == -1)
    {
		perror("mq_unlink dispatcher error\n");
        return 1;
    }

    if(sem_destroy(&connection_sem) != 0)
    {
		perror("sem_destroy connection sem error\n");
		return 1;
    }
    return 0;
}

static void * Dispatcher_dispatch()
{
    while(sem_wait(&connection_sem) == -1)
    {
        if(errno == EINTR)
        {
            continue;
        }
        else
        {
            perror("sem_wait connection sem error\n");
            printf("end dispatcher loop\n");
            return NULL;
        }
    }
    printf("exit sem_wait\n");

    while(gui_connected) //tant que la connexion est active
    {
        char* header = Postman_readMessage(HEADER_SIZE); //attente de la réception d'une trame
        char* frame;
        uint16_t frameSize;

        if(header != NULL)
        {
            Protocol_hexdump(header, HEADER_SIZE);
            printf("start decoding the header\n");
        }
        else
        {
            frame = NULL;
            frameSize = 0;
        }

        Decoded_Header decodedHeader = Protocol_decodeHeader(header);
        printf("Cmd : %d\n", decodedHeader.cmdId);
        printf("Taille : %d\n", decodedHeader.size);
        if(decodedHeader.cmdId > NB_CMD) //Cmd en dehors des limites, header non conforme
        {
            frame = NULL;
            frameSize = 0;
        }
        else
        {
            frameSize = decodedHeader.size - HEADER_SIZE; //decodedHeader.size est la taille totale de la trame
            frame = Postman_readMessage(frameSize);
            Protocol_hexdump(frame, frameSize);
        }

        if(frame != NULL) //On ne décode que si le header est conforme
        {
            Decoded_Frame decodedFrame = Protocol_decode(frame, frameSize); //décodage de la trame en une instance de la structure Decoded_Frame
            printf("decoded\n");
            char* arg1 = NULL; 
            char* arg2 = NULL;
            char* arg3 = NULL;
            char* arg4 = NULL;
            char* arg5 = NULL;

            //initilaisation des potentiels arguments selon la variable de nombre d'arguments nbArgs
            if (decodedFrame.nbArgs >= 1) {
                arg1 = malloc(decodedFrame.lenArgs[0] + 1); 
                if (arg1 == NULL) {
                    printf("Erreur d'allocation de mémoire pour arg1\n");
                }
            }
            if (decodedFrame.nbArgs >= 2) {
                arg2 = malloc(decodedFrame.lenArgs[1] + 1); 
                if (arg2 == NULL) {
                    printf("Erreur d'allocation de mémoire pour arg2\n");
                }
            }
            if (decodedFrame.nbArgs >= 3) {
                arg3 = malloc(decodedFrame.lenArgs[2] + 1); 
                if (arg3 == NULL) {
                    printf("Erreur d'allocation de mémoire pour arg3\n");
                }
            }
            if (decodedFrame.nbArgs >= 4) {
                arg4 = malloc(decodedFrame.lenArgs[3] + 1); 
                if (arg4 == NULL) {
                    printf("Erreur d'allocation de mémoire pour arg4\n");
                }
            }
            if (decodedFrame.nbArgs == 5) {
                arg5 = malloc(decodedFrame.lenArgs[4] + 1); 
                if (arg5 == NULL) {
                    printf("Erreur d'allocation de mémoire pour arg5\n");
                }
            }
            
            //répartition des trames selon leur identifiant de commande
            switch(decodedHeader.cmdId)
            {
                case CMD_ERR:
                    printf("%s : ERROR\n", __FILE__);
                    break;
                case CMD_CHANGE_MODE:
                    printf("%s : CHANGE_MODE\n", __FILE__);
                    break;
                case CMD_STANDBY:
                    printf("%s : STANDBY\n", __FILE__);
                    break;
                case CMD_CHECK_PASSWORD:
                    printf("%s : CHECK_PASSWORD\n", __FILE__);
                    break;
                case CMD_SET_PICTURE:
                    printf("%s : SET_PICTURE\n", __FILE__);
                    break;
                case CMD_ADD_USER:
                    printf("%s : ADD_USER\n", __FILE__);
                    break;
                case CMD_MODIFY_USER:
                    printf("%s : MODIFY_USER\n", __FILE__);
                    break;
                case CMD_REMOVE_USER:
                    printf("%s : REMOVE_USER\n", __FILE__);
                    break;
                case CMD_ASK_ALL_USERS:
                    printf("%s : ASK_ALL_USERS\n", __FILE__);
                    break;
                case CMD_ASK_SEARCH_USER:
                    printf("%s : ASK_SEARCH_USER\n", __FILE__);
                    break;
                default:
                    printf("Cmd_Id n°%d non traité pour l'instant\n", decodedHeader.cmdId);
                    break;
            }
            //libération des mémoires allouées aux arguments et à la trame
            if(arg1 != NULL) free(arg1);
            if(arg2 != NULL) free(arg2);
            if(arg3 != NULL) free(arg3);
            if(arg4 != NULL) free(arg4);
            if(arg5 != NULL) free(arg5);

            if(frame != NULL) free(frame); //libération de la mémoire allouée aux chaînes de caractères contenant la trame et le header
        }
        free(header);
    }
    printf("end dispatcher loop\n");

    return NULL;
}

void Dispatcher_setConnected(bool state)
{
    if(state)
    {
        gui_connected = true;
        sem_post(&connection_sem);
        printf("sem_post\n");
    }
    else
    {
        gui_connected = false;
    }
}
