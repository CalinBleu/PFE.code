#ifndef GUI_H
#define GUI_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Méthode publique permettant l’allumage de l’écran.
 * 
 */
void Gui_screenOn();

/**
 * @brief Méthode publique permettant d’éteindre l’écran.
 * 
 */
void Gui_screenOff();

/**
 * @brief Méthode publique permettant de valider le mot de passe inscrit dans le champ mot de passe.
 * 
 */
void Gui_validatePassword(bool result);

/**
 * @brief Méthode publique permettant d’attribuer un tag RFID au nouvel Utilisateur.
 * 
 */
void Gui_setTag(char* idTag);

/**
 * @brief Méthode publique permettant d’afficher les écrans d’authentification. 
 * 
 */
void Gui_displayHomeScreen(AuthResult result);



/**
 * @brief Méthode publique permettant de demander la vérification du mot de passe entré par l'utilisateur
 * 
 */
void Gui_askConnexion(char* password);

/**
 * @brief Méthode publique permettant de notifier le changement de mode
 * 
 * @param mode 
 */
void Gui_changeMode(Mode mode);

/**
 * @brief Méthode publique permettant de sauvegarder l'ajout d'un utilisateur
 * 
 * @param user 
 */
void Gui_userAdded(User user);

/**
 * @brief Méthode publique permettant de sauvegarder la modification d'un utilisateur
 * 
 * @param user 
 */
void Gui_userModified(User user, char* idTag);

/**
 * @brief Méthode publique permettant de sauvegarder la suppression d'un utilisateur
 * 
 * @param idTag 
 */
void Gui_userRemoved(char* idTag);

/**
 * @brief Méthode publique permettant de notifier la demande de recherche d'un utilisateur
 * 
 * @param searchField 
 */
void Gui_userSearched(char* searchField);

#ifdef __cplusplus
}
#endif

#endif // GUI_H
