#ifndef DISPATCHER_H
#define DISPATCHER_H

/*
 * INCLUDES
 */
#include <stdbool.h>
#include <stdint.h>

/*
 * DEFINES
 */

/*
 * TYPEDEF
 */

/*
 * PUBLIC FUNCTIONS
 */

/**
 * @brief Fonction d'allocation de la mémoire liée au dispatcheur AOP
 *
 * @return uint8_t Potentiel code d'erreur
 */
uint8_t dispatcher_new();

/**
 * @brief Fonction d'initialisation et de démarrage du dispatcheur AOP
 * 
 * @return uint8_t Potentiel code d'erreur
 */
uint8_t dispatcher_start();

/**
 * @brief Fonction d'arrêt du dispatcheur AOP
 */
void dispatcher_stop();

/**
 * @brief Fonction de libération de la mémoire liée au dispatcheur AOP
 *
 * @return uint8_t Potentiel code d'erreur
 */
uint8_t dispatcher_free();

/**
 * @brief Fonction de notification de l'état de la connexion avec AOP
 * 
 * @param state état de la connexion
 */
void dispatcher_setConnected(bool state);

#endif //DISPATCHER_H
