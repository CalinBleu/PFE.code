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
 * @brief Fonction d'allocation de la mémoire liée au dispatcheur
 *
 * @return uint8_t Potentiel code d'erreur
 */
uint8_t Dispatcher_new();

/**
 * @brief Fonction d'initialisation et de démarrage du dispatcheur
 * 
 * @return uint8_t Potentiel code d'erreur
 */
uint8_t Dispatcher_start();

/**
 * @brief Fonction d'arrêt du dispatcheur
 */
void Dispatcher_stop();

/**
 * @brief Fonction de libération de la mémoire liée au dispatcheur
 *
 * @return uint8_t Potentiel code d'erreur
 */
uint8_t Dispatcher_free();

/**
 * @brief Fonction de notification de l'état de la connexion avec GUI
 * 
 * @param state état de la connexion
 */
void Dispatcher_setConnected(bool state);

#endif //DISPATCHER_H
