
/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick 6.5
import QtMultimedia
import Visiolock

Item {
    id: accueilresult
    width: Constants.width
    height: Constants.height

    property alias text_auth : text_auth
    property alias text_msg : text_message

    Rectangle {
        width: Constants.width
        height: Constants.height
        border.color: "#171F69"
        border.width: 4
        color: "white"

        Image {
            id: logo
            x: 342
            y: 25
            width: 117
            height: 15
            source: "../content/images/thales_logo.png"
            fillMode: Image.PreserveAspectFit
        }

        Image {
            id: top
            x: 488
            y: -52
            width: 308
            height: 308
            source: "../content/images/top_image.png"
            fillMode: Image.PreserveAspectFit
        }

        Image {
            id: bottom
            x: 4
            y: 114
            width: 372
            height: 362
            source: "../content/images/bottom_image.png"
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
