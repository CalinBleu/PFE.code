/*
 * INCLUDES
 */
#if 0
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
#include "gui.h"
#include "guard.h"
#include "brain.h"
#include "manager.h"

/*
 * DEFINES
 */
#define NAME_MQ_BOX_GUI  "/mq_gui" //Boîte aux lettres liée à Gui
#define MQ_MSG_COUNT 10

/*
 * TYPEDEF
 */
typedef enum  {S_FORGET = 0, S_STANDBY, S_TAG_SCANNED, S_HOME_DETECTION, S_UNKNOWN_USER, S_ACCESS_AUTHORIZED, S_ADMIN_HOME_DETECTION, S_ACCESS_DENIED, S_MANAGEMENT_PASSWORD, S_MANAGEMENT_SETTINGS, S_MODIFY_USER_POPUP, S_MODIFY_USER_MANAGEMENT, S_MODIFY_USER_PICTURE, S_NEW_USER_RFID, S_NEW_USER_PICTURE, S_NEW_USER_NAMES, S_NEW_USER_ACCESS_ROLE, S_NEW_USER_SUMMARY, S_DELETE_USER, S_DEATH, STATE_NB} State;
typedef enum  {E_SCREEN_ON = 0, E_SCREEN_OFF, E_USER_TAG_OK, E_USER_TAG_UNKNOWN, E_FACE_UNKNOWN, E_USER_ALLOWED, E_USER_TAG_DENIED, E_ADMIN_TAG, E_ADMIN_MODE, E_QUIT_ADMIN_MODE, E_MANAGEMENT_USER, E_ASK_MODIFY_USER, E_MODIFY_USER, E_ASK_ADD_USER, E_ADD_USER, E_VALIDATE, E_CANCEL, E_ASK_TAKE_PICTURE, E_TAKE_PICTURE, E_ASK_DELETE_USER, E_REMOVE_USER, E_STOP, EVENT_NB} Event;
typedef enum  {A_NOP = 0, A_SCREEN_ON, A_SCREEN_OFF, A_USER_TAG_OK, A_USER_TAG_UNKNOWN, A_FACE_UNKNOWN, A_USER_ALLOWED, A_USER_TAG_DENIED, A_ADMIN_TAG, A_ADMIN_MODE, A_QUIT_ADMIN_MODE, A_MANAGEMENT_USER, A_ASK_MODIFY_USER, A_MODIFY_USER, A_ASK_ADD_USER, A_ADD_USER, A_VALIDATE, A_ASK_TAKE_PICTURE, A_TAKE_PICTURE, A_CANCEL, A_ASK_DELETE_USER, A_REMOVE_USER, A_STOP} Action ;

/*
 * brief Structure des transitions liées à GUI. Etat de destination et action à réaliser.
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
static void Gui_mqReceive(MqMsg * aMsg);

static void Gui_mqSend(MqMsg * aMsg);

static void Gui_performAction(Action anAction, MqMsg * aMsg);

static void * Gui_run(void * aParam);

static void Gui_timer_launch();

static void Gui_cancel_timer();

static void Gui_timeout(union sigval val);

/*
 * LOCAL VARIABLES
 */
static pthread_t gui_thread;
static mqd_t gui_mq;
static timer_t screen_lock_timer;

static User userToAdd;
static char* passwordField;
static char* searchField;

/**
 * brief Tableau des transitions de la machine à états de GUI
 * 
 */
static Transition mySm [STATE_NB-1][EVENT_NB] = //Transitions état-action selon l'état courant et l'évènement reçu
{
    [S_STANDBY][E_SCREEN_ON] = {S_TAG_SCANNED, A_SCREEN_ON},
    [S_TAG_SCANNED][E_USER_TAG_OK] = {S_HOME_DETECTION, A_USER_TAG_OK},
    [S_TAG_SCANNED][E_USER_TAG_UNKNOWN] = {S_UNKNOWN_USER, A_USER_TAG_UNKNOWN},
    [S_TAG_SCANNED][E_USER_TAG_DENIED] = {S_ACCESS_DENIED, A_USER_TAG_DENIED},
    [S_TAG_SCANNED][E_ADMIN_TAG] = {S_ADMIN_HOME_DETECTION, A_ADMIN_TAG},
    [S_HOME_DETECTION][E_FACE_UNKNOWN] = {S_UNKNOWN_USER, A_FACE_UNKNOWN},
    [S_HOME_DETECTION][E_USER_ALLOWED] = {S_ACCESS_AUTHORIZED, A_USER_ALLOWED},
    [S_ADMIN_HOME_DETECTION][E_FACE_UNKNOWN] = {S_UNKNOWN_USER, A_FACE_UNKNOWN},
    [S_ADMIN_HOME_DETECTION][E_USER_ALLOWED] = {S_ACCESS_AUTHORIZED, A_USER_ALLOWED},
    [S_ADMIN_HOME_DETECTION][E_ADMIN_MODE] = {S_MANAGEMENT_PASSWORD, A_ADMIN_MODE},
    [S_MANAGEMENT_PASSWORD][E_QUIT_ADMIN_MODE] = {S_ADMIN_HOME_DETECTION, A_QUIT_ADMIN_MODE},
    [S_MANAGEMENT_PASSWORD][E_MANAGEMENT_USER] = {S_MANAGEMENT_SETTINGS, A_MANAGEMENT_USER},
    [S_MANAGEMENT_SETTINGS][E_QUIT_ADMIN_MODE] = {S_ADMIN_HOME_DETECTION, A_QUIT_ADMIN_MODE},
    [S_MANAGEMENT_SETTINGS][E_ASK_ADD_USER] = {S_NEW_USER_RFID, A_ASK_ADD_USER},
    [S_MANAGEMENT_SETTINGS][E_ASK_MODIFY_USER] = {S_MODIFY_USER_POPUP, A_ASK_MODIFY_USER},
    [S_MANAGEMENT_SETTINGS][E_ASK_DELETE_USER] = {S_DELETE_USER, A_ASK_DELETE_USER},
    [S_NEW_USER_RFID][E_CANCEL] = {S_MANAGEMENT_SETTINGS, A_CANCEL},
    [S_NEW_USER_RFID][E_VALIDATE] = {S_NEW_USER_PICTURE, A_VALIDATE},
    [S_NEW_USER_PICTURE][E_CANCEL] = {S_MANAGEMENT_SETTINGS, A_CANCEL},
    [S_NEW_USER_PICTURE][E_TAKE_PICTURE] = {S_NEW_USER_NAMES, A_TAKE_PICTURE},
    [S_NEW_USER_NAMES][E_CANCEL] = {S_MANAGEMENT_SETTINGS, A_CANCEL},
    [S_NEW_USER_NAMES][E_VALIDATE] = {S_NEW_USER_ACCESS_ROLE, A_VALIDATE},
    [S_NEW_USER_ACCESS_ROLE][E_CANCEL] = {S_MANAGEMENT_SETTINGS, A_CANCEL},
    [S_NEW_USER_ACCESS_ROLE][E_VALIDATE] = {S_NEW_USER_SUMMARY, A_VALIDATE},
    [S_NEW_USER_SUMMARY][E_CANCEL] = {S_MANAGEMENT_SETTINGS, A_CANCEL},
    [S_NEW_USER_SUMMARY][E_ADD_USER] = {S_MANAGEMENT_SETTINGS, A_ADD_USER},
    [S_DELETE_USER][E_CANCEL] = {S_MANAGEMENT_SETTINGS, A_CANCEL},
    [S_DELETE_USER][E_REMOVE_USER] = {S_MANAGEMENT_SETTINGS, A_REMOVE_USER},
    [S_MODIFY_USER_POPUP][E_CANCEL] = {S_MANAGEMENT_SETTINGS, A_CANCEL},
    [S_MODIFY_USER_POPUP][E_VALIDATE] = {S_MODIFY_USER_MANAGEMENT, A_VALIDATE},
    [S_MODIFY_USER_MANAGEMENT][E_CANCEL] = {S_MANAGEMENT_SETTINGS, A_CANCEL},
    [S_MODIFY_USER_MANAGEMENT][E_MODIFY_USER] = {S_MANAGEMENT_SETTINGS, A_MODIFY_USER},
    [S_MODIFY_USER_MANAGEMENT][E_ASK_TAKE_PICTURE] = {S_MODIFY_USER_PICTURE, A_ASK_TAKE_PICTURE},
    [S_MODIFY_USER_PICTURE][E_CANCEL] = {S_MODIFY_USER_MANAGEMENT, A_CANCEL},
    [S_MODIFY_USER_PICTURE][E_TAKE_PICTURE] = {S_MODIFY_USER_MANAGEMENT, A_TAKE_PICTURE},
    //standby
    [S_TAG_SCANNED][E_SCREEN_OFF] = {S_STANDBY, A_SCREEN_OFF},
    [S_HOME_DETECTION][E_SCREEN_OFF] = {S_STANDBY, A_SCREEN_OFF},
    [S_ADMIN_HOME_DETECTION][E_SCREEN_OFF] = {S_STANDBY, A_SCREEN_OFF},
    [S_MANAGEMENT_PASSWORD][E_SCREEN_OFF] = {S_STANDBY, A_SCREEN_OFF},
    [S_MANAGEMENT_SETTINGS][E_SCREEN_OFF] = {S_STANDBY, A_SCREEN_OFF},
    [S_NEW_USER_ACCESS_ROLE][E_SCREEN_OFF] = {S_STANDBY, A_SCREEN_OFF},
    [S_NEW_USER_NAMES][E_SCREEN_OFF] = {S_STANDBY, A_SCREEN_OFF},
    [S_NEW_USER_SUMMARY][E_SCREEN_OFF] = {S_STANDBY, A_SCREEN_OFF},
    [S_NEW_USER_RFID][E_SCREEN_OFF] = {S_STANDBY, A_SCREEN_OFF},
    [S_NEW_USER_PICTURE][E_SCREEN_OFF] = {S_STANDBY, A_SCREEN_OFF},
    [S_DELETE_USER][E_SCREEN_OFF] = {S_STANDBY, A_SCREEN_OFF},
    [S_MODIFY_USER_POPUP][E_SCREEN_OFF] = {S_STANDBY, A_SCREEN_OFF},
    [S_MODIFY_USER_PICTURE][E_SCREEN_OFF] = {S_STANDBY, A_SCREEN_OFF},
    [S_MODIFY_USER_MANAGEMENT][E_SCREEN_OFF] = {S_STANDBY, A_SCREEN_OFF},
    [S_UNKNOWN_USER][E_SCREEN_OFF] = {S_STANDBY, A_SCREEN_OFF},
    [S_ACCESS_AUTHORIZED][E_SCREEN_OFF] = {S_STANDBY, A_SCREEN_OFF},
    [S_ACCESS_DENIED][E_SCREEN_OFF] = {S_STANDBY, A_SCREEN_OFF},
    //stop GUI
    [S_TAG_SCANNED][E_STOP] = {S_DEATH, A_STOP},
    [S_HOME_DETECTION][E_STOP] = {S_DEATH, A_STOP},
    [S_ADMIN_HOME_DETECTION][E_STOP] = {S_DEATH, A_STOP},
    [S_MANAGEMENT_PASSWORD][E_STOP] = {S_DEATH, A_STOP},
    [S_MANAGEMENT_SETTINGS][E_STOP] = {S_DEATH, A_STOP},
    [S_NEW_USER_ACCESS_ROLE][E_STOP] = {S_DEATH, A_STOP},
    [S_NEW_USER_SUMMARY][E_STOP] = {S_DEATH, A_STOP},
    [S_NEW_USER_NAMES][E_STOP] = {S_DEATH, A_STOP},
    [S_NEW_USER_PICTURE][E_STOP] = {S_DEATH, A_STOP},
    [S_NEW_USER_RFID][E_STOP] = {S_DEATH, A_STOP},
    [S_DELETE_USER][E_STOP] = {S_DEATH, A_STOP},
    [S_MODIFY_USER_POPUP][E_STOP] = {S_DEATH, A_STOP},
    [S_MODIFY_USER_PICTURE][E_STOP] = {S_DEATH, A_STOP},
    [S_MODIFY_USER_MANAGEMENT][E_STOP] = {S_DEATH, A_STOP},
    [S_UNKNOWN_USER][E_STOP] = {S_DEATH, A_STOP},
    [S_ACCESS_AUTHORIZED][E_STOP] = {S_DEATH, A_STOP},
    [S_ACCESS_DENIED][E_STOP] = {S_DEATH, A_STOP},
};

/**
 * brief Fonction permettant d'initialiser la boite aux lettres
 * 
 */
int Gui_new(void)
{
    int check;
    struct mq_attr mqa = {
        .mq_maxmsg = MQ_MSG_COUNT,
        .mq_msgsize = sizeof(MqMsg),
        .mq_flags = 0,
    }; //Attributs de la bal
    
    check = mq_unlink(NAME_MQ_BOX_GUI);
    if((check == -1) && (errno =! ENOENT))
    {
        printf("mq_unlink Gui error\n");
    }
    brain_mq = mq_open(NAME_MQ_BOX_GUI, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, &mqa); //ouverture de la bal
    if(gui_mq == -1)
    {
		perror("mq_open Gui error\n");
		return 1;
    }

	struct sigevent event;

	event.sigev_notify = SIGEV_THREAD;
	event.sigev_value.sival_ptr = NULL;
	event.sigev_notify_function = Gui_timeout;
	event.sigev_notify_attributes = NULL;

	if (timer_create(CLOCK_REALTIME, &event, &screen_lock_timer) != 0) {
		perror("timer_create");
        return 1;
	}

    return 0;
}

/**
 * brief Fonction permettant de lancer le thread GUI
 */
int Gui_start(void)
{
    if(pthread_create(&gui_thread, NULL, Gui_run, NULL) != 0)
    {
        fprintf(stderr, "pthread_create Gui error\n");
        fflush(stderr);
        return 1;
    }
    return 0;
}

/**
 * brief Fonction permettant de gérer le timeout de l'écran
 */
static void Gui_timeout(union sigval val)
{
	MqMsg msg = {.data.event = E_TIMEOUT};
	Gui_mqSend(&msg);
}

/**
 * brief Fonction permettant de libérer la mémoire et la boite aux lettres
 */
int Gui_free(void)
{
    int check;
    check = mq_close(gui_mq); //fermeture de la boîte aux lettres
    if(check == -1)
    {
		perror("mq_close Gui error\n");
		return 1;
    }
    check = mq_unlink(NAME_MQ_BOX_GUI); //suppression de la boîte aux lettres
    if(check == -1)
    {
		perror("mq_unlink Gui error\n");
		return 1;
    }
    return 0;
}

/**
 * brief Fonction permettant de recevoir un message de la boîte aux lettres
 * 
 * param aMsg Le message à recevoir
 */
static void Gui_mqReceive(MqMsg * aMsg)
{
	int check;
	check = mq_receive(gui_mq, aMsg->buffer, sizeof(MqMsg), NULL); //réception d'un message de la mq
    if (check != sizeof(MqMsg))
    {
        perror("Error receiving message via gui mqueue");
    }
}

/**
 * brief Fonction permettant d'envoyer un message dans la boîte aux lettres
 * 
 * param aMsg Le message à envoyer
 */
static void Gui_mqSend(MqMsg * aMsg)
{
    int check;
    check = mq_send(gui_mq, aMsg->buffer, sizeof(MqMsg), 0); //envoi d'un message à la mq
    if (check == -1) {
        perror("Error sending message via gui mqueue");
    }
    else
    {
        printf("Message sent via gui mq\n");
    }
}

/**
 * brief Fonction permettant d'effectuer une action en fonction de l'évènement reçu
 * 
 * param anAction L'action à effectuer
 * param aMsg Le message associé
 */
static void Gui_performAction(Action anAction, MqMsg * aMsg)
{
    switch (anAction)
    {
        case A_NOP: 
            break;
        case A_SCREEN_ON:
            printf("%s : A_SCREEN_ON\n", __FILE__);
            //allumer l'écran
            break;
        case A_SCREEN_OFF:
            printf("%s : A_SCREEN_OFF\n", __FILE__);
            //éteindre l'écran
            break;
        case A_USER_TAG_OK:
            printf("%s : A_USER_TAG_OK\n", __FILE__);
            //displayScreen(HOME_DETECTION)
            break;
        case A_USER_TAG_UNKNOWN:
            printf("%s : A_USER_TAG_UNKNOWN\n", __FILE__);
            //displayScreen(HOME_UNKNOWN_USER)
            break;
        case A_FACE_UNKNOWN:
            printf("%s : A_FACE_UNKNOWN\n", __FILE__);
            //displayScreen(HOME_UNKNOWN_USER)
            break;
        case A_USER_ALLOWED:
            printf("%s : A_USER_ALLOWED\n", __FILE__);
            //displayScreen(HOME_ACCESS_AUTHORIZED)
            break;
        case A_USER_TAG_DENIED:
            printf("%s : A_USER_TAG_DENIED\n", __FILE__);
            //displayScreen(HOME_ACCESS_DENIED)
            break;
        case A_ADMIN_TAG:
            printf("%s : A_ADMIN_TAG\n", __FILE__);
            //displayScreen(HOME_ADMIN_DETECTION)
            break;
        case A_ADMIN_MODE:
            printf("%s : A_ADMIN_MODE\n", __FILE__);
            //displayScreen(ADMIN_LOGIN)
            break;
        case A_QUIT_ADMIN_MODE:
            printf("%s : A_QUIT_ADMIN_MODE\n", __FILE__);
            //displayScreen(HOME_ADMIN_DETECTION)
            break;
        case A_MANAGEMENT_USER:
            printf("%s : A_MANAGEMENT_USER\n", __FILE__);
            //displayScreen(MANAGEMENT_USERS)
            Brain_changeMode(ADMIN);
            break;
        case A_ASK_MODIFY_USER:
            printf("%s : A_ASK_MODIFY_USER\n", __FILE__);
            //displayScreen(POP_UP_MODIFY)
            break;
        case A_MODIFY_USER:
            printf("%s : A_MODIFY_USER\n", __FILE__);
            Manager_modifyUser(userToAdd);
            break;
        case A_ASK_ADD_USER:
            printf("%s : A_ASK_ADD_USER\n", __FILE__);
            break;
        case A_ADD_USER:
            Manager_addUser(userToAdd);
            printf("%s : A_ADD_USER\n", __FILE__);
            break;
        case A_VALIDATE:
            printf("%s : A_VALIDATE\n", __FILE__);
            break;
        case A_ASK_TAKE_PICTURE:
            printf("%s : A_ASK_TAKE_PICTURE\n", __FILE__);
            break;
        case A_TAKE_PICTURE:
            printf("%s : A_TAKE_PICTURE\n", __FILE__);
            break;
        case A_CANCEL:
            printf("%s : A_CANCEL\n", __FILE__);
            break;
        case A_ASK_DELETE_USER:
            printf("%s : A_ASK_DELETE_USER\n", __FILE__);
            break;
        case A_REMOVE_USER:
            printf("%s : A_REMOVE_USER\n", __FILE__);
            break;
        case A_STOP:
            printf("%s : A_STOP\n", __FILE__);
            break;
    }
}

/**
 * brief Fonction permettant de lancer la machine à états de GUI
 * 
 * param aParam Paramètre de la fonction
 */
static void * Gui_run(void * aParam)
{
    printf("Gui run\n");
	MqMsg msg;
    State myState = S_INIT;
    Transition * myTrans;
	while (myState != S_DEATH)
    {
        Gui_mqReceive(&msg);
        myTrans = &mySm[myState][msg.data.event];
        printf("%s : %d : myState : %d\n", __FILE__, __LINE__, myState);
        printf("%s : %d : event : %d\n", __FILE__, __LINE__, msg.data.event);
        printf("%s : %d : dest state : %d\n", __FILE__, __LINE__, myTrans->destinationState);
        printf("%s : %d : action : %d\n", __FILE__, __LINE__, myTrans->action);
        if (myTrans->destinationState != S_FORGET)
        {
            Gui_performAction(myTrans->action, &msg);
            myState = myTrans->destinationState;
        }
        else
        {
            printf("Gui : Event %d ignored\n", msg.data.event);
        }
    }
    printf("end of thread Gui\n");
    return NULL;
}

/**
 * brief Fonction permettant de lancer le timer de l'écran
 */
static void Gui_timer_launch()
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

/**
 * brief Fonction permettant d'annuler le timer de l'écran
 */
static void Gui_cancel_timer()
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

/**
 * brief Fonction qui envoie l'évènement E_SCREEN_ON à la machine à états de GUI
 */
void Gui_screenOn()
{
    MqMsg msg = {.data.event = E_SCREEN_ON}; //envoi de l'évènement E_SCREEN_ON via mq
	Gui_mqSend(&msg);
}

/**
 * brief Fonction qui envoie l'évènement E_SCREEN_OFF à la machine à états de GUI
 */
void Gui_screenOff()
{
    MqMsg msg = {.data.event = E_SCREEN_OFF}; //envoi de l'évènement E_SCREEN_OFF via mq
	Gui_mqSend(&msg);
}

/**
 * brief Fonction qui envoie l'évènement E_USER_TAG_OK à la machine à états de GUI
 */
void Gui_askAddUser()
{
    MqMsg msg = {.data.event = E_ASK_ADD_USER}; //envoi de l'évènement E_ASK_ADD_USER via mq
	Gui_mqSend(&msg);
}

/**
 * brief Fonction qui envoie l'évènement E_ASK_MODIFY_USER à la machine à états de GUI
 * 
 * param user L'utilisateur à modifier
 */
void Gui_askModifyUser(User user)
{
    MqMsg msg = {.data.event = E_ASK_MODIFY_USER}; //envoi de l'évènement E_ASK_MODIFY_USER via mq
	Gui_mqSend(&msg);
}

/**
 * brief Fonction qui envoie l'évènement E_ASK_DELETE_USER à la machine à états de GUI
 * 
 * param user L'utilisateur à supprimer
 */
void Gui_askDeleteUser(User user)
{
    MqMsg msg = {.data.event = E_ASK_DELETE_USER}; //envoi de l'évènement E_ASK_DELETE_USER via mq
	Gui_mqSend(&msg);
}

/**
 * brief Fonction permettant de cherhcer un utilisateur dans la base de données
 *
 */
void Gui_setSearchField(char* search)
{
    searchField = search;
}

/**
 * brief Fonction qui envoie l'évènement E_VALIDATE à la machine à états de GUI
 */
void Gui_validate()
{
    MqMsg msg = {.data.event = E_VALIDATE}; //envoi de l'évènement E_VALIDATE via mq
	Gui_mqSend(&msg);
}

/**
 * brief Fonction qui envoie l'évènement E_CANCEL à la machine à états de GUI
 *
 */
void Gui_cancel()
{
    MqMsg msg = {.data.event = E_CANCEL}; //envoi de l'évènement E_CANCEL via mq
	Gui_mqSend(&msg);
}

/**
 * brief Fonction qui permet d'inscrire un mot de passe dans le champ mot de passe
 * 
 * param password Le mot de passe à inscrire
 */
void Gui_setFieldPassword(char* password)
{
    passwordField = password;
}

/**
 * brief Fonction qui envoie l'évènement E_MANAGEMENT_USER  ou E_ADMIN_MODE à la machine à états de GUI
 * 
 * param result Le résultat de la vérification du mot de passe 
 */
void Gui_validatePassword(bool result)
{
    MqMsg msg;
    if(true == result){
        msg.data.event = E_MANAGEMENT_USER; //envoi de l'évènement E_MANAGEMENT_USER via mq
    } else {
        msg.data.event = E_ADMIN_MODE; //envoi de l'évènement E_ADMIN_MODE via mq
    }
	Gui_mqSend(&msg);
}

/**
 * brief Fonction qui permet de définir le nom de l'utilisateur à ajouter
 * 
 * param name Le nom de l'utilisateur à ajouter
 */
void Gui_setName(char* name)
{
    userToAdd.name = name;
}

/**
 * brief Fonction qui permet de définir le prénom de l'utilisateur à ajouter
 * 
 * param firstname Le prénom de l'utilisateur à ajouter
 */
void Gui_setFirstName(char* firstname)
{
    userToAdd.firstName = firstname;
}

/**
 * brief Fonction qui permet de définir le rôle de l'utilisateur à ajouter
 * 
 * param role Le rôle de l'utilisateur à ajouter
 */
void Gui_setRole(Role role)
{
    userToAdd.role = role;
}

/**
 * brief Fonction qui permet de définir les accès de l'utilisateur à ajouter
 * 
 * param access Les accès de l'utilisateur à ajouter
 */
void Gui_setAccess(Access access)
{
    userToAdd.access = access;
}

/**
 * brief Fonction qui permet de définir le tag RFID de l'utilisateur à ajouter
 * 
 * param idTag Le tag RFID de l'utilisateur à ajouter
 */
void Gui_setTag(char* idTag)
{
    userToAdd.idTag = idTag;
}

/**
 * brief Fonction qui permet de définir la photo de l'utilisateur à ajouter
 * 
 * param picture La photo de l'utilisateur à ajouter
 */
void Gui_setPicture(Picture picture)
{
    userToAdd.picture = picture
}

/**
 * brief Fonction qui envoie l'évènement E_ASK_TAKE_PICTURE à la machine à états de GUI
 */
void Gui_askTakePicture()
{
    MqMsg msg = {.data.event = E_ASK_TAKE_PICTURE}; //envoi de l'évènement E_ASK_TAKE_PICTURE via mq
	Gui_mqSend(&msg);
}

/**
 * brief Fonction qui envoie l'évènement E_ADMIN_MODE à la machine à états de GUI
 */
 */
void Gui_adminMode()
{
    MqMsg msg = {.data.event = E_ADMIN_MODE}; //envoi de l'évènement E_ADMIN_MODE via mq
	Gui_mqSend(&msg);
}

/**
 * brief Fonction qui envoie l'évènement E_QUIT_ADMIN_MODE à la machine à états de GUI
 */
 */
void Gui_quitAdminMode()
{
    MqMsg msg = {.data.event = E_QUIT_ADMIN_MODE}; //envoi de l'évènement E_QUIT_ADMIN_MODE via mq
	Gui_mqSend(&msg);
}

/**
 * brief Fonction qui envoie l'évènement E_USER_MODIFY8USER à la machine à états de GUI
 * 
 * param user L'utilisateur à modifier
 */
void Gui_displayHomeScreen(AuthResult result)
{
    MqMsg msg;
    if(result) {
        msg.data.event = E_ASK_MODIFY_USER; //envoi de l'évènement E_ASK_MODIFY_USER via mq
    }
	Gui_mqSend(&msg);
}

/**
 * brief TODO
 */
static void Gui_displayScreen(ScreenId screenId)
{
    //TODO
}

#endif





