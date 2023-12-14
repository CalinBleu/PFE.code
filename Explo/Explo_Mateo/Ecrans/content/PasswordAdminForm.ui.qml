
/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick 6.5
import Ecrans
import QtMultimedia
import QtQuick.Controls 6.5

Item {
    id: it
    width: Constants.width
    height: Constants.height

    property alias button_password: admin_password
    property alias button_quit: admin_quit

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
            id: admin
            x: 317
            y: 161
            text: qsTr("Administrateur")
            font.pixelSize: 24
            font.styleName: "Bold"
            color: "#171F69"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            transformOrigin: Item.Center
        }

        Text {
            id: password
            x: 201
            y: 226
            text: qsTr("Mot de passe :")
            font.pixelSize: 24
            font.styleName: "Bold"
            color: "#171F69"
        }

        Button {
            id: admin_quit
            x: 614
            y: 403
            width: 167
            height: 48
            contentItem: Text {
                text: qsTr("Quitter")
                color: "#FF6D6D"
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
            id: admin_password
            x: 317
            y: 297
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

        TextInput {
            id: password_input
            x: 368
            y: 226
            width: 254
            height: 35
            text: qsTr("password")
            color: "#171F69"
            font.styleName: "Bold"
            font.pixelSize: 14
        }
    }

    states: [
        State {
            name: "clicked"
        }
    ]
}
