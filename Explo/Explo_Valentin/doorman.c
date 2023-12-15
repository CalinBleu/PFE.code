/*
 * INCLUDES
 */
#include <stdio.h>
#include <gpiod.h>
#include <unistd.h>
#include "doorman.h"
#include <signal.h>
#include <time.h>
#include <pthread.h>


/*
 * DEFINES
 */

#define	LED_ORANGE	22
#define LED_RED 	27	
#define LOCK		17
#define TIMER_LED   5

/*
 * STRUCT
 */
const char *chipnameLock = "gpiochip0";
struct gpiod_chip *chipLock;
struct gpiod_line *lineLock;

const char *chipnamRed = "gpiochip0";
struct gpiod_chip *chipRed;
struct gpiod_line *lineRed;

const char *chipnameOrange = "gpiochip0";
struct gpiod_chip *chipOrange;
struct gpiod_line *lineOrange;

static timer_t led_timer;

/*
 * LOCAL FUNCTIONS
 */

static void Doorman_init();

static void Doorman_close();

static void led_timer_launch();

static void led_cancel_timer();

static void led_timeout(union sigval val);




/******************************************************************************/

/**
 * brief Fonction d'intialisation des ports GPIO
 *
 */
static void Doorman_init(){
	chipLock = gpiod_chip_open_by_name(chipnameLock);
	lineLock = gpiod_chip_get_line(chipLock, LOCK);
	gpiod_line_request_output(lineLock, "ex", 0);

	chipOrange = gpiod_chip_open_by_name(chipnameOrange);
	lineOrange = gpiod_chip_get_line(chipOrange, LED_ORANGE);
	gpiod_line_request_output(lineOrange, "ex", 0);

	chipRed = gpiod_chip_open_by_name(chipnamRed);
	lineRed = gpiod_chip_get_line(chipRed, LED_RED);
	gpiod_line_request_output(lineRed, "ex", 0);


    struct sigevent event;

	event.sigev_notify = SIGEV_THREAD;
	event.sigev_value.sival_ptr = NULL;
	event.sigev_notify_function = led_timeout;
	event.sigev_notify_attributes = NULL;

   if (timer_create(CLOCK_REALTIME, &event, &led_timer) != 0) {
		perror("timer_create");
	}
}

/**
 * brief Fonction permettant d'ouvrir la porte
 */
void Doorman_open(){
	gpiod_line_set_value(lineLock, 1);
}
/**
 * brief Fonction permettant de fermer la porte
 */
static void Doorman_close(){
    gpiod_line_set_value(lineLock, 0); 
}


/**
 * brief Fonction d'initialisation du timer TIMER_LED
 * 
 */
static void led_timer_launch()
{
	struct itimerspec itimer;

	itimer.it_interval.tv_sec = 0;
	itimer.it_interval.tv_nsec = 0;
	itimer.it_value.tv_sec = TIMER_LED;
	itimer.it_value.tv_nsec = 0;

	if (timer_settime(led_timer, 0, &itimer, NULL) != 0) {
		perror("timer_settime launch");
	}
}

/**
 * brief Fonction d'annulation du timer TIMER_LED
 * 
 */
static void led_cancel_timer()
{
	struct itimerspec itimer;

	itimer.it_interval.tv_sec = 0;
	itimer.it_interval.tv_nsec = 0;
	itimer.it_value.tv_sec = 0;
	itimer.it_value.tv_nsec = 0;

	if (timer_settime(led_timer, 0, &itimer, NULL) != 0) {
		perror("timer_settime cancel");
	}
}

/**
 * brief Fonction permettant l'allumage de la LED correspondant à l'évènement "l'utilisateur est 
 * reconnu mais n'a pas le droit d'accéder au bâtiment"
 * 
 */
void Doorman_userDenied() {
    gpiod_line_set_value(lineOrange, 1);
    led_timer_launch();    
}

/**
 * brief Fonction permettant l'allumage de la LED correspondant à l'évènement "l'utilisateur n'est pas reconnu"
 * 
 */
void Doorman_userUnknown() {
    gpiod_line_set_value(lineRed, 1);
    led_timer_launch();
}


/**
 * brief Fonction de timeout du timer pour l'exctinction des LED
 * 
 * param val Le signal déclenché par le timeout
 */
static void led_timeout(union sigval val){
    gpiod_line_set_value(lineRed, 0);
    gpiod_line_set_value(lineOrange, 0);
}

int main(){
    Doorman_init();

    while (1)
    {
        Doorman_open();
        Doorman_close();
        Doorman_userDenied();
        Doorman_userUnknown();
    }
    
}