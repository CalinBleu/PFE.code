/*
 * INCLUDES
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "protocol.h"
#include "proxyGui.h"
#include "postman.h"
#include "common.h"

/*
 * DEFINES
 */

/*
 * TYPEDEF
 */

/*
 * LOCAL FUNCTIONS
 */

/*
 * LOCAL VARIABLES
 */

/*******************************************************************************************/

void ProxyGui_screenOn()
{
    char* frame = Protocol_encode(CMD_SCREEN_ON,0,NULL);

    Postman_askSendMessage(frame); 

    Protocol_destroyFrame(frame, NULL);
}

void ProxyGui_screenOff()
{
    char* frame = Protocol_encode(CMD_SCREEN_OFF,0,NULL);

    Postman_askSendMessage(frame); 

    Protocol_destroyFrame(frame, NULL);
}

void ProxyGui_displayHomeScreen(AuthResult result)
{
    char* allArgs = malloc(sizeof(result) + 1);
    allArgs[0] = '\0';
    sprintf(allArgs, "%d", result);

    char* frame = Protocol_encode(CMD_DISPLAY_HOME_SCREEN,1,allArgs);

    Postman_askSendMessage(frame); 

    Protocol_destroyFrame(frame, allArgs);
}

void ProxyGui_setTag(char* idTag)
{
    char* allArgs = malloc(strlen(idTag) + 1);
    allArgs[0] = '\0';
    sprintf(allArgs, "%s", idTag);

    char* frame = Protocol_encode(CMD_SET_TAG,1,allArgs);

    Postman_askSendMessage(frame); 

    Protocol_destroyFrame(frame, allArgs);
}

void ProxyGui_validatePassword(bool result)
{
    char* allArgs = malloc(sizeof(result) + 1);
    allArgs[0] = '\0';
    sprintf(allArgs, "%d", result);

    char* frame = Protocol_encode(CMD_VALIDATE_PASSWORD,1,allArgs);

    Postman_askSendMessage(frame); 

    Protocol_destroyFrame(frame, allArgs);
}