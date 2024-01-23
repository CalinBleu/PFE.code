#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "protocol.h"
#include "common.h"


int main()
{
    int un_socket;
    
    struct sockaddr_in adresse_du_serveur;

    un_socket = socket(PF_INET, SOCK_STREAM, 0);

    adresse_du_serveur.sin_family = AF_INET;
    adresse_du_serveur.sin_port = htons(1253);
    adresse_du_serveur.sin_addr = *((struct in_addr *)gethostbyname ("127.0.0.1")->h_addr_list[0]);

    connect(un_socket, (struct sockaddr *)&adresse_du_serveur, sizeof(adresse_du_serveur));

    Mode mode = MODE_ADMIN;
    char* allArgs = malloc(sizeof(mode) + 1);
    allArgs[0] = '\0';
    sprintf(allArgs, "%d", mode);

    char* frame = Protocol_encode(CMD_CHANGE_MODE,1,allArgs); //encodage de la trame

    int frameLen = Protocol_getIntLength(frame[1], frame[0]);
    printf("framelen : %d\n", frameLen);
    int quantite_envoyee = write(un_socket, frame, frameLen);

    Protocol_destroyFrame(frame, allArgs);

    close(un_socket);

    return 0;
}