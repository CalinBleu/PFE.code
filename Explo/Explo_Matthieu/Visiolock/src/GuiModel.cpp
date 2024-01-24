#include "GuiModel.h"

extern "C" {
#include "common.h"
#include "src/brain/brain.h"
#include "src/archivist/archivist.h"
#include "src/manager/manager.h"
#include "src/doorman/doorman.h"
#include "src/rfid/rfid.h"
#include "src/ai/ai.h"
#include "src/gui/gui.h"
#include <unistd.h>
}

GuiModel::GuiModel(QObject *parent) : QObject{parent} {}

void GuiModel::Gui_start(){

    Rfid_new();
    Rfid_start();
    AI_new();
    AI_start();
    Brain_startVisiolock();
    Doorman_init();
    Archivist_open();

    sleep(20000);

    Archivist_clearImages();
    Archivist_close();
    Brain_stopVisiolock();
    Rfid_stop();
    AI_stop();
    sleep(5000);
    Brain_free();
    Rfid_free();
    AI_free();
}

void GuiModel::Gui_askConnexionController(char* password) {
    Gui_askConnexion(password);
}

void GuiModel::Gui_changeModeController(Mode mode) {
    Gui_changeMode(mode);
}

void GuiModel::Gui_userAddedController(User user) {
    Gui_userAdded(user);
}

void GuiModel::Gui_userModifiedController(User user) {
    Gui_userModified(user);
}

void GuiModel::Gui_userRemovedController(char* idTag) {
    Gui_userRemoved(idTag);
}

void GuiModel::Gui_userSearchedController(char* searchField) {
    Gui_userSearched(searchField);
}

