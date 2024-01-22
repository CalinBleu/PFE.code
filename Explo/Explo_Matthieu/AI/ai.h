#ifndef AI_H
#define AI_H

#define CAMERA_IMAGES_PATH ("../camera_images/last_shot.jpg")
#define CAMERA_IMAGES_PATH_LEN (sizeof(CAMERA_IMAGES_PATH))

//TODO remove
#define DATASET_PATH "/home/matthieu/Documents/PFE/PFE.code/Explo/Explo_Matthieu/pictures/image_0.jpg"
#define DATASET_PATH1 "/home/matthieu/Documents/PFE/PFE.code/Explo/Explo_Matthieu/pictures/image_1.jpg"


int AI_new(void);

int AI_start(void);

int AI_free(void);

void AI_startRecognition(char *picturePath);

void AI_stopRecognition(void);


#endif //AI_H