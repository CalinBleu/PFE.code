#ifndef __AI_H__
#define __AI_H__

//TODO remove
#define DATASET_PATH "/home/pi/Documents/facial_recognition/dataset/Matthieu/image_0.jpg"

int AI_new(void);

int AI_start(void);

int AI_free(void);

int AI_startRecognition(char *picturePath);

int AI_stopRecognition(void);


#endif //__AI_H__