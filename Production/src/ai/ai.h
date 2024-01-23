#ifndef AI_H
#define AI_H

#define CAMERA_IMAGES_PATH ("../camera_images/last_shot.jpg")

int AI_new(void);

int AI_start(void);

int AI_free(void);

int AI_stop(void);

void AI_startRecognition(char *picturePath);

void AI_stopRecognition(void);


#endif //AI_H