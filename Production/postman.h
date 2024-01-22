#ifndef POSTMAN_H
#define POSTMAN_H

/*
 * INCLUDES
 */

#include <stdint.h>
#include <stdarg.h>

/*
 * DEFINES
 */

#define SERVER_PORT 1253
#define MAX_PENDING_CONNECTIONS 10
#define MAX_TCP_LENGTH 1500

/*
 * TYPEDEF
 */

/*
 * PUBLIC FUNCTIONS
 */

/**
 * @brief Fonction d'allocation de la mémoire liée au facteur
 *  
 * @return uint8_t Potentiel code d'erreur
 */
uint8_t Postman_new();

/**
 * @brief Fonction d'initialisation et de démarrage du facteur
 * 
 * @return uint8_t Potentiel code d'erreur
 */
uint8_t Postman_start();

/**
 * @brief Fonction d'arrêt du serveur C.
 */
void Postman_stop(void);

/**
 * @brief Fonction de libération de la mémoire liée au facteur
 *  
 * @return uint8_t Potentiel code d'erreur
 */
uint8_t Postman_free();

/**
 * @brief Fonction de lecture de la socket.
 * @param [in] size La taille du message à lire
 * @return char* Le message lu par le facteur
 */
char* Postman_readMessage(int size);

/**
 * @brief Fonction d'écriture sur la socket.
 * @param [in] frame La trame de donnée à envoyer.
 */
void Postman_askSendMessage(char* frame);

#endif //POSTMAN_H
