#ifndef PROXY_GUI_H
#define PROXY_GUI_H

#include "common.h"

void ProxyGui_screenOn();

void ProxyGui_screenOff();

void ProxyGui_displayHomeScreen(AuthResult result);

void ProxyGui_setTag(char* idTag);

void ProxyGui_validatePassword(bool result); 

#endif //PROXY_GUI_H