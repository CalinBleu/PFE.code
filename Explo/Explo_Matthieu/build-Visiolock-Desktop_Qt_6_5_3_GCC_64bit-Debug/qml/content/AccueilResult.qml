import QtQuick 6.5
import Visiolock
import QtMultimedia

AccueilResultForm{

    text_auth.text: {
        if (GuiController.authResult === 4) {
            qsTr("Accès autorisé")
        } else if (GuiController.authResult === 3) {
            qsTr("Accès refusé")
        } else if (GuiController.authResult === 2) {
            qsTr("Utilisateur non reconnu")
        }
    }

    text_auth.color: {
        if (GuiController.authResult === 4) {
            "#49DB24"
        } else if (GuiController.authResult === 3) {
            "#FF6D6D"
        } else if (GuiController.authResult === 2) {
            "#FF9901"
        }
    }

    text_msg.text: {
        if (GuiController.authResult === 4) {
            qsTr("Bonne journée <Nom Utilisateur>")
        } else if (GuiController.authResult === 3) {
            qsTr("Bonne journée <Nom Utilisateur>")
        } else if (GuiController.authResult === 2) {
            qsTr("Veuillez réessayer ou contacter l'administrateur")
        }
    }
}
