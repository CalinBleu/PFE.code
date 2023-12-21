#ifndef GUI_H
#define GUI_H

void GUI_screenOn();

void GUI_screenOff();

void GUI_askAddUser();

void GUI_askModifyUser(User user);

void GUI_askDeleteUser(User user);

void GUI_setSearchField(char* search);

void GUI_validate();

void GUI_cancel();

void GUI_setFieldPassword(char* password);

void GUI_validatePassword(bool result);

void GUI_setName(char* name);

void GUI_setFirstName(char* firstName);

void GUI_setRole(Role role);

void GUI_setAccess(Access access);

void GUI_setTag(char* idTag);

void GUI_setPicture(Picture picture);

void GUI_askTakePicture();

void GUI_adminMode();

void GUI_quitAdminMode();

void GUI_displayHomeScreen(AuthResult result);

/**
 * @brief Fonction d'initialisation de l'objet GUI
 * 
 */
int GUI_new();

/**
 * @brief Fonction de démarrage de l'objet GUI
 * 
 */
int GUI_start();

/**
 * @brief Fonction de destruction de l'objet GUI
 * 
 */
int GUI_free();

#endif //GUI_H