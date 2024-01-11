/*
 * INCLUDES
 */
#include <stdio.h>
#include <gpiod.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>
#include "common.h"
#include "doorman.h"


/*
 * DEFINES
 */

#define	LED_ORANGE	22
#define LED_RED 	27	
#define LOCK		17
#define TIMER_LED   5
#define TIMER_LOCK	5

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
static timer_t lock_timer;

/*
 * LOCAL FUNCTIONS
 */

static void Doorman_close();

static void Doorman_led_timer_launch();

static void Doorman_led_timeout(union sigval val);

static void Doorman_lock_timer_launch();

static void Doorman_lock_timeout(union sigval val);




/******************************************************************************/

/**
 * brief Fonction d'intialisation des ports GPIO
 *
 */
void Doorman_init(){
	#if TARGET
	chipLock = gpiod_chip_open_by_name(chipnameLock);
	lineLock = gpiod_chip_get_line(chipLock, LOCK);
	gpiod_line_request_output(lineLock, "ex", 0);

	chipOrange = gpiod_chip_open_by_name(chipnameOrange);
	lineOrange = gpiod_chip_get_line(chipOrange, LED_ORANGE);
	gpiod_line_request_output(lineOrange, "ex", 0);

	chipRed = gpiod_chip_open_by_name(chipnamRed);
	lineRed = gpiod_chip_get_line(chipRed, LED_RED);
	gpiod_line_request_output(lineRed, "ex", 0);
	#endif

    struct sigevent event;

	event.sigev_notify = SIGEV_THREAD;
	event.sigev_value.sival_ptr = NULL;
	event.sigev_notify_function = Doorman_led_timeout;
	event.sigev_notify_attributes = NULL;

   	if (timer_create(CLOCK_REALTIME, &event, &led_timer) != 0) {
		perror("timer_create");
	}

	struct sigevent event2;

	event2.sigev_notify = SIGEV_THREAD;
	event2.sigev_value.sival_ptr = NULL;
	event2.sigev_notify_function = Doorman_lock_timeout;
	event2.sigev_notify_attributes = NULL;

   	if (timer_create(CLOCK_REALTIME, &event2, &lock_timer) != 0) {
		perror("timer_create");
	}
}

/**
 * brief Fonction permettant d'ouvrir la porte
 */
void Doorman_open(){
	#if TARGET	
	gpiod_line_set_value(lineLock, 1);
	#endif
	Doorman_lock_timer_launch();
	printf("open");
}
/**
 * brief Fonction permettant de fermer la porte
 */
static void Doorman_close(){
	#if TARGET	
    gpiod_line_set_value(lineLock, 0);
	#endif
	printf("close"); 
}


/**
 * brief Fonction d'initialisation du timer TIMER_LED
 * 
 */
static void Doorman_led_timer_launch()
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

static void Doorman_lock_timer_launch()
{
	struct itimerspec itimer;

	itimer.it_interval.tv_sec = 0;
	itimer.it_interval.tv_nsec = 0;
	itimer.it_value.tv_sec = TIMER_LOCK;
	itimer.it_value.tv_nsec = 0;

	if (timer_settime(lock_timer, 0, &itimer, NULL) != 0) {
		perror("timer_settime launch");
	}
}

/**
 * brief Fonction permettant l'allumage de la LED correspondant à l'évènement "l'utilisateur est 
 * reconnu mais n'a pas le droit d'accéder au bâtiment"
 * 
 */
void Doorman_userDenied() {
	#if TARGET	
    gpiod_line_set_value(lineOrange, 1);
	#endif
    Doorman_led_timer_launch();    
}

/**
 * brief Fonction permettant l'allumage de la LED correspondant à l'évènement "l'utilisateur n'est pas reconnu"
 * 
 */
void Doorman_userUnknown() {
	#if TARGET	
    gpiod_line_set_value(lineRed, 1);
	#endif
    Doorman_led_timer_launch();
}


/**
 * brief Fonction de timeout du timer pour l'exctinction des LED
 * 
 * param val Le signal déclenché par le timeout
 */
static void Doorman_led_timeout(union sigval val){
	#if TARGET	
    gpiod_line_set_value(lineRed, 0);
    gpiod_line_set_value(lineOrange, 0);
	#endif
}

static void Doorman_lock_timeout(union sigval val){
	Doorman_close();
}


