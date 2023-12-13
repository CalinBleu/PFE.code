#ifndef BRAIN_H
#define BRAIN_H

void Brain_startVisiolock();

void Brain_stopVisiolock();

void Brain_standBy();

void Brain_tagReaded(char* idTag);

void Brain_changeMode(Mode mode);

void Brain_faceAnalysed(bool recognized);

#endif //BRAIN_H