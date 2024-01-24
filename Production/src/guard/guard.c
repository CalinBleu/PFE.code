#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>
#include "common.h"
#include "sha256.h"
#include "guard.h"
#include "brain/brain.h"
#include "archivist/archivist.h"
#include "gui/gui.h"
#include "ai/ai.h"

/*
 * LOCAL FUNCTIONS
 */
static void Guard_timer_launch();

static void Guard_cancel_timer();

static void Guard_timeout(union sigval val);

static timer_t ai_process_timer;

/*
 * PUBLIC FUNCTIONS
 */

void Guard_checkPassword(char* password, char* idTag ){
    //comparaison entre le mot de passe hashé de la BDD et le hash du mot de passe en paramètre

    char password_hash[SHA256_HEX_SIZE];

    char *hash = Archivist_getPassword(idTag);

    sha256_hex(password, strlen(password), password_hash);

    if(strcmp(password_hash, hash)== 0){
        printf("Correct Password\n");
        Gui_validatePassword(true);
    }
    else{
        printf("Incorrect Password\n");
        Gui_validatePassword(false);
    }
}

AuthResult Guard_checkTag(char* idTag){
    //détermination des droits de l'utilisateur en fonction de son tag, son role et ses accès
    Role role = Archivist_getRole(idTag);
    Access* access = Archivist_getAccess(idTag);

    if(role == ADMIN){
        return ADMIN_TAG;
    }
    else if (role == USER)
    {
        if(*access[CURRENT_ZONE] == true){
            return USER_TAG_OK;
        }
        else{
            return USER_TAG_DENIED;
        }
    }
    else{
        return USER_TAG_UNKNOWN;
    }
    
}

void Guard_resultRecognition(AuthResult authResult){
    Guard_cancel_timer();
    if(authResult == ALLOWED){
        Brain_faceAnalysed(true);
    }
    else{
        Brain_faceAnalysed(false);
    }
}

void Guard_checkFace(char* idTag){
	struct sigevent event;

	event.sigev_notify = SIGEV_THREAD;
	event.sigev_value.sival_ptr = NULL;
	event.sigev_notify_function = Guard_timeout;
	event.sigev_notify_attributes = NULL;

	if (timer_create(CLOCK_REALTIME, &event, &ai_process_timer) != 0) {
		perror("timer_create");
	}
    Picture picture = Archivist_getPicture(idTag);
    AI_startRecognition(picture);
    Guard_timer_launch();
}


/**
 * @brief Fonction qui lance le timer
 */
static void Guard_timer_launch()
{
	struct itimerspec itimer;

	itimer.it_interval.tv_sec = 0;
	itimer.it_interval.tv_nsec = 0;
	itimer.it_value.tv_sec = 15;
	itimer.it_value.tv_nsec = 0;

	if (timer_settime(ai_process_timer, 0, &itimer, NULL) != 0) {
		perror("timer_settime launch");
	}
}

/**
 * @brief Fonction qui stoppe le timer
 */
static void Guard_cancel_timer()
{
	struct itimerspec itimer;

	itimer.it_interval.tv_sec = 0;
	itimer.it_interval.tv_nsec = 0;
	itimer.it_value.tv_sec = 0;
	itimer.it_value.tv_nsec = 0;

	if (timer_settime(ai_process_timer, 0, &itimer, NULL) != 0) {
		perror("timer_settime cancel");
	}
}

/**
 * @brief Fonction qui gère le timeout du timer
 */
static void Guard_timeout(union sigval val)
{
    AI_stopRecognition();
}


