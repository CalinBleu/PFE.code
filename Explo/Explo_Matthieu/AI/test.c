#include "ai.h"
#include <stdlib.h>
#include <stdio.h>

int main(void){

    char *path[200];
    path = DATASET_FOLDER;
    sprintf(path, "/Matthieu/image_0.png");
    AI_startRecognition(path);

    return 0;
}