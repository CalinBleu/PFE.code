#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "common.h"
#include "brain.h"

int main(){
    //simulation scénario
    Brain_startVisiolock();

    sleep(2);
    Brain_tagReaded("1234");
    sleep(1);
    //MqMsg msg = {.data.event = E_USER_TAG_OK}; 
	//Brain_mqSend(&msg);
    sleep(2);
    Brain_faceAnalysed(true);
    sleep(5);
    //msg.data.event = E_TIMEOUT_LOCK; 
	//Brain_mqSend(&msg);
    sleep(5);
    Brain_tagReaded("1235");
    sleep(1);
    //msg.data.event = E_USER_TAG_DENIED; 
	//Brain_mqSend(&msg);
    sleep(5);


    Brain_free();
}