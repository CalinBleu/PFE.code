
/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick 6.5
import Ecrans
import QtMultimedia
import QtQuick.Studio.Components
import QtQuick.Controls 6.5

Item {
    id: it
    width: Constants.width
    height: Constants.height

    property alias button_validate: validate
    property alias button_cancel: cancel

    Rectangle {
        id: rectangle
        x: 0
        y: 0
        width: Constants.width
        height: Constants.height
        border.color: "#FF6D6D"
        border.width: 4
        color: "white"

        Image {
            id: nom
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
            id: admin
            x: 300
            y: 63
            text: qsTr("Nouvel Utilisateur")
            font.pixelSize: 24
            font.styleName: "Bold"
            color: "#171F69"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            transformOrigin: Item.Center
        }

        Button {
            id: cancel
            x: 30
            y: 53
            width: 167
            height: 48
            contentItem: Text {
                text: qsTr("Annuler")
                color: "#171F69"
                font.styleName: "Bold"
                font.pixelSize: 14
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle {
                color: "#ffffff"
                radius: 17
                border.color: "#171F69"
                border.width: 4
            }
        }

        Button {
            id: validate
            x: 317
            y: 285
            width: 167
            height: 48
            contentItem: Text {
                text: qsTr("Valider")
                color: "#171F69"
                font.styleName: "Bold"
                font.pixelSize: 14
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle {
                color: "#ffffff"
                radius: 17
                border.color: "#171F69"
                border.width: 4
            }
        }

        TextField {
            id: role_input
            x: 286
            y: 172
            width: 228
            height: 33
            text: qsTr("Rôle")
            color: "#171F69"
            font.styleName: "Bold"
            font.pixelSize: 14

            Image {
                x: 197
                y: 5
                width: 24
                height: 24
                source: "../content/images/down.png"
                fillMode: Image.PreserveAspectFit
            }
        }

        TextField {
            id: access_input
            x: 286
            y: 223
            width: 228
            height: 33
            text: qsTr("Accès")
            color: "#171F69"
            font.styleName: "Bold"
            font.pixelSize: 14

            Image {
                x: 197
                y: 5
                width: 24
                height: 24
                source: "../content/images/down.png"
                fillMode: Image.PreserveAspectFit
            }
        }
    }

    states: [
        State {
            name: "clicked"
        }
    ]
}
