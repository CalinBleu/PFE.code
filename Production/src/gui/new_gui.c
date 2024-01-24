/*
 * INCLUDES
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "common.h"
#include "gui/gui.h"
#include "guard/guard.h"
#include "brain/brain.h"
#include "manager/manager.h"


//c vers qml
void Gui_displayHomeScreen(AuthResult result)
{
    //GuiController_displayHomeScreen(result);
}

void Gui_validatePassword(bool result)
{
    //GuiController_passResult(result);
}

void Gui_screenOn()
{
    //GuiController_activateScreen(true);
}

void Gui_screenOff()
{
    //GuiController_activateScreen(false);
}

void Gui_setTag(char* idTag)
{
    //GuiController_displayNewUserTag(idTag);
}

//qml vers c
void Gui_askConnexion(char* password)
{
    char tag[20];
    Brain_getCurrentTag(tag, sizeof(tag));
    Guard_checkPassword(password, tag);
}

void Gui_changeMode(Mode mode)
{
    Brain_changeMode(mode);
}

void Gui_userAdded(User user)
{
    Manager_addUser(user);
}

void Gui_userModified(User user)
{
    char tag[20];
    Brain_getCurrentTag(tag, sizeof(tag));
    Manager_modifyUser(user, tag);
}

void Gui_userRemoved(char* idTag)
{
    Manager_removeUser(idTag);
}

void Gui_userSearched(char* searchField)
{
    Manager_searchUser(searchField);
}




