#include <stdio.h>
#include <gpiod.h>
#include <unistd.h>

//gcc test.c -lgpiod -o  test

#define	LED_ORANGE	22
#define LED_ROUGE 	27	
#define SERRURE		17

const char *chipnameSerrure = "gpiochip0";
struct gpiod_chip *chipSerrure;
struct gpiod_line *lineSerrure;

const char *chipnameRouge = "gpiochip0";
struct gpiod_chip *chipRouge;
struct gpiod_line *lineRouge;

const char *chipnameOrange = "gpiochip0";
struct gpiod_chip *chipOrange;
struct gpiod_line *lineOrange;

static void Doorman_init(){
	chipSerrure = gpiod_chip_open_by_name(chipnameSerrure);
	lineSerrure = gpiod_chip_get_line(chipSerrure, SERRURE);
	gpiod_line_request_output(lineSerrure, "ex", 0);

	chipOrange = gpiod_chip_open_by_name(chipnameOrange);
	lineOrange = gpiod_chip_get_line(chipOrange, LED_ORANGE);
	gpiod_line_request_output(lineOrange, "ex", 0);

	chipRouge = gpiod_chip_open_by_name(chipnameRouge);
	lineRouge = gpiod_chip_get_line(chipRouge, LED_ROUGE);
	gpiod_line_request_output(lineRouge, "ex", 0);
}


void Doorman_open(){
	gpiod_line_set_value(lineSerrure, 1);
	sleep(5);
}

void Doorman_close(){
    gpiod_line_set_value(lineSerrure, 0);
	sleep(5);  
}

void Doorman_userDenied(){
    //led orange
    gpiod_line_set_value(lineOrange, 1);
	sleep(5);
	gpiod_line_set_value(lineOrange, 0);

}

void Doorman_userUnknown(){
    //led red
    gpiod_line_set_value(lineRouge, 1);
	sleep(5);
	gpiod_line_set_value(lineRouge, 0);

}


int main(){

	Doorman_init();

	while(1){
		Doorman_open();
		Doorman_close();
		Doorman_userDenied();
		Doorman_userUnknown();
		//sleep(1);
	}	

}
