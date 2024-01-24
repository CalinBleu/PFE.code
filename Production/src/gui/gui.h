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
 * @brief Méthode publique permettant de faire la demande d’ajout d’un nouvel Utilisateur.
 * 
 */
void Gui_askAddUser();

/**
 * @brief Méthode publique permettant de faire la demande de modification d’un utilisateur de la base de données.
 * 
 */
void Gui_askModifyUser(User user);

/**
 * @brief Méthode publique permettant de faire la demande de supprimer un Utilisateur de la base de données.
 * 
 */
void Gui_askDeleteUser(User user);

/**
 * @brief Méthode publique permettant d’inscrire un nom d’Utilisateur dans le champ de recherche.
 * 
 */
void Gui_setSearchField(char* search);

/**
 * @brief Méthode publique permettant de valider une action.
 * 
 */
void Gui_validate();

/**
 * @brief Méthode publique permettant d’annuler une action.
 * 
 */
void Gui_cancel();

/**
 * @brief Méthode publique permettant d’inscrire un mot de passe dans le champ mot de passe.
 * 
 */
void Gui_setFieldPassword(char* password);

/**
 * @brief Méthode publique permettant de valider le mot de passe inscrit dans le champ mot de passe.
 * 
 */
void Gui_validatePassword(bool result);

/**
 * @brief Méthode publique permettant d’inscrire un nouveau nom d’Utilisateur dans le champ nom utilisateur.
 * 
 */
void Gui_setName(char* name);

/**
 * @brief Méthode publique permettant d’inscrire un nouveau prénom d’Utilisateur dans le champ prénom utilisateur.
 * 
 */
void Gui_setFirstName(char* firstname);

/**
 * @brief Méthode publique permettant de choisir le rôle voulu pour le nouvel Utilisateur.
 * 
 */
void Gui_setRole(Role role);

/**
 * @brief Méthode publique permettant de choisir les accès autorisés du nouvel Utilisateur.
 * 
 */
void Gui_setAccess(Access access);

/**
 * @brief Méthode publique permettant d’attribuer un tag RFID au nouvel Utilisateur.
 * 
 */
void Gui_setTag(char* idTag);

/**
 * @brief Méthode publique permettant d’ajouter une photo du nouvel Utilisateur.
 * 
 */
void Gui_setPicture(Picture picture);

/**
 * @brief Méthode publique permettant de faire la demande de prise d’une photo.
 * 
 */
void Gui_askTakePicture();

/**
 * @brief Méthode publique permettant de basculer vers le mode administrateur.
 * 
 */
void Gui_adminMode();

/**
 * @brief Méthode publique permettant de quitter le mode administrateur.
 * 
 */
void Gui_quitAdminMode();

/**
 * @brief Méthode publique permettant d’afficher les écrans d’authentification. 
 * 
 */
void Gui_displayHomeScreen(AuthResult result);

void Gui_askConnexion(char* password);

void Gui_changeMode(Mode mode);

void Gui_userAdded(User user);

void Gui_userModified(User user);

void Gui_userRemoved(char* idTag);

void Gui_userSearched(char* searchField);

/**
 * @brief Fonction d'initialisation de l'objet Gui. 
 * 
 */
int Gui_new();

/**
 * @brief Fonction de démarrage de l'objet Gui. 
 * 
 */
int Gui_start();

/**
 * @brief Fonction de destruction de l'objet Gui. 
 * 
 */
int Gui_free();

#ifdef __cplusplus
}
#endif

#endif // GUI_H
