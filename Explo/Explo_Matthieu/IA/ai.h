#ifndef __AI_H__
#define __AI_H__

#define DATASET_PATH "/home/pi/Documents/facial_recognition/dataset/Matthieu/image_0.jpg"

void AI_init(void);

int AI_startRecognition(char *picturePath);
void AI_stopRecognition(void);


#endif //__AI_H__