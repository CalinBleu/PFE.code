
/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick 2.5
import Ecrans
import QtMultimedia
import QtQuick.Studio.Components

Item {
    id: accueilresult
    width: Constants.width
    height: Constants.height

    Rectangle {
        width: Constants.width
        height: Constants.height
        border.color: "#171F69"
        border.width: 4
        color: "white"

        Image {
            id: name
            x: 342
            y: 25
            width: 117
            height: 15
            source: "../content/images/Thales 3.png"
            fillMode: Image.PreserveAspectFit
        }

        Image {
            id: image_top
            x: 488
            y: -52
            width: 308
            height: 308
            source: "../content/images/Group 5.png"
            fillMode: Image.PreserveAspectFit
        }

        Image {
            id: image_bot
            x: 4
            y: 114
            width: 372
            height: 362
            source: "../content/images/Group 6.png"
            fillMode: Image.PreserveAspectFit
        }

        Text {
            id: text_auth
            x: 308
            y: 191
            text: qsTr("Accès autorisé")
            //text: qsTr("Accès refusé")
            //text: qsTr("Utilisateur non reconnu")
            font.pixelSize: 26
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            transformOrigin: Item.Center
            font.styleName: "Bold"
            font.bold: true
            color: "#49DB24"
            //color: "#FF6D6D"  rouge
            //color: "#FF9901"  orange
        }

        Text {
            id: text_message
            x: 196
            y: 235
            text: qsTr("Bonne journée <Nom Utilisateur>")
            //text: qsTr("Veuillez réessayer ou contacter l'administrateur")
            font.pixelSize: 26
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            transformOrigin: Item.Center
            font.styleName: "Bold"
            font.bold: true
            color: "#171F69"
        }
    }
}
