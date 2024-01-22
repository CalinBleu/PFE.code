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
#include "postman.h"
#include "common.h"


int main()
{
    int un_socket;
    
    struct sockaddr_in adresse_du_serveur;

    un_socket = socket(PF_INET, SOCK_STREAM, 0);

    adresse_du_serveur.sin_family = AF_INET;
    adresse_du_serveur.sin_port = htons(SERVER_PORT);
    adresse_du_serveur.sin_addr = *((struct in_addr *)gethostbyname ("127.0.0.1")->h_addr_list[0]);

    connect(un_socket, (struct sockaddr *)&adresse_du_serveur, sizeof(adresse_du_serveur));


    char* frame = Protocol_encode(CMD_STANDBY,0,NULL); //encodage de la trame

    Postman_askSendMessage(frame); //demande d'envoi du message au facteur

    Protocol_destroyFrame(frame, NULL);

    close(un_socket);

    return 0;
}