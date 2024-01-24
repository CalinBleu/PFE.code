import QtQuick 6.5
import Visiolock
import QtMultimedia

AccueilResultForm{

    text_auth.text: {
        if (myData.myString === "1") {
            qsTr("Accès autorisé")
        } else if (myData.myString === "2") {
            qsTr("Accès refusé")
        }
    }

    text_auth.color: {
        if (myData.myString === "1") {
            "#49DB24"
        } else if (myData.myString === "2") {
            "#FF6D6D"
        }
    }

    text_msg.text: {
        if (myData.myString === "1") {
            qsTr("Bonne journée <Nom Utilisateur>")
        } else if (myData.myString === "2") {
            qsTr("Bonne journée <Nom Utilisateur>")
        }
    }
}
