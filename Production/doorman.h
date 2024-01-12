#ifndef DOORMAN_H
#define DOORMAN_H

/**
 * brief Fonction permettant d'ouvrir la porte
 * 
 */
void Doorman_open();

/**
 * brief Fonction permettant l'allumage de la LED correspondant à l'évènement "l'utilisateur est 
 * reconnu mais n'a pas le droit d'accéder au bâtiment"
 * 
 */
void Doorman_userDenied();

/**
 * brief Fonction permettant l'allumage de la LED correspondant à l'évènement "l'utilisateur n'est pas reconnu"
 * 
 */
void Doorman_userUnknown();

void Doorman_init();

#endif //DOORMAN_H