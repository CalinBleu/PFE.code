#include "gui.h"
#include "common.h"

/**
 * @brief
 *
 */
enum class State {
    S_STANDBY,
    S_TAG_SCANNED,
    S_HOME_DETECTION,
    S_ADMIN_HOME_DETECTION,
    S_ACCESS_DENIED,
    S_ACCESS_AUTHORIZED,
    S_UNKNOWN_USER,
    S_MANAGEMENT_PASSWORD,
    S_MANAGEMENT_SETTINGS,
    S_NEW_USER,
    S_MODIFY_USER,
    S_DELETE_USER
};

// Interface de la machine à états
/**
 * @brief
 *
 */
class StateMachine {
public:
    /**
     * @brief
     *
     */
    StateMachine() {
        // Initialisation du tableau de transitions
        transitions[State::S_STANDBY] = std::bind(&StateMachine::screenOn(), this);
        transitions[State::S_TAG_SCANNED] = std::bind(&StateMachine::displayHomeScreen(), this);
        transitions[State::State3] = std::bind(&StateMachine::transitionState3, this);
    }

    /**
     * @brief
     *
     */
    void processEvent() {
        // Appel de la fonction associée à l'état courant
        transitions[currentState]();
    }

private:
    // Définition des actions pour chaque état

    //Transition de S_STANDBY à S_TAG_SCANNED
    /**
     * @brief
     *
     */
    void screenOn() {
        currentState = State::S_TAG_SCANNED;
    }

    /**
     * @brief
     *
     */
    void displayHomeScreen(result) {
        switch (result) {
            case AuthResult::USER_TAG_OK:
                currentState = State::S_HOME_DETECTION;
                break;
            case AuthResult::USER_TAG_UNKNOWN:
                currentState = State::S_UNKNOWN_USER;
                break;
            case AuthResult::USER_TAG_DENIED:
                currentState = State::S_ACCESS_DENIED;
                break;
            case AuthResult::FACE_UNKNOWN:
                currentState = State::S_UNKNOWN_USER;
                break;
            case AuthResult::ALLOWED:
                currentState = State::S_ACCESS_AUTHORIZED;
                break;
        }
    }

    State currentState = State::S_STANDBY;
    std::array<std::function<void()>, 3> transitions; // Tableau de fonctions de transition
};
