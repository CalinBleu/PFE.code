
/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick 6.5
import QtMultimedia
import QtQuick.Controls 6.5
import Visiolock

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

        Rectangle {
            id: rectangle_photo
            x: 360
            y: 139
            width: 344
            height: 285
            color: "#ffffff"
            radius: 17
            border.color: "#171F69"
            border.width: 4

            Image {
                id: photo
                x: 0
                y: 0
                width: parent.width
                height: parent.height
            }
        }

        Rectangle {
            id: rectangle_name
            x: 95
            y: 143
            width: 228
            height: 33
            color: "#D9D9D9"

            Text {
                id: name
                x: 5
                y: 0
                width: 218
                height: 33
                text: qsTr("Nom")
                color: "#171F69"
                font.styleName: "Bold"
                font.pixelSize: 14
                verticalAlignment: Text.AlignVCenter
            }
        }

        Rectangle {
            id: rectangle_firstname
            x: 95
            y: 187
            width: 228
            height: 33
            color: "#D9D9D9"

            Text {
                id: fisrtname
                x: 5
                y: 0
                width: 218
                height: 33
                text: qsTr("Prénom")
                color: "#171F69"
                font.styleName: "Bold"
                font.pixelSize: 14
                verticalAlignment: Text.AlignVCenter
            }
        }

        Rectangle {
            id: rectangle_role
            x: 95
            y: 231
            width: 228
            height: 33
            color: "#D9D9D9"

            Text {
                id: role
                x: 5
                y: 0
                width: 218
                height: 33
                text: qsTr("Rôle")
                color: "#171F69"
                font.styleName: "Bold"
                font.pixelSize: 14
                verticalAlignment: Text.AlignVCenter
            }
        }

        Rectangle {
            id: rectangle_rfid
            x: 95
            y: 275
            width: 228
            height: 33
            color: "#D9D9D9"

            Text {
                id: rfid
                x: 5
                y: 0
                width: 218
                height: 33
                text: qsTr("RFID")
                color: "#171F69"
                font.styleName: "Bold"
                font.pixelSize: 14
                verticalAlignment: Text.AlignVCenter
            }
        }

        Rectangle {
            id: rectangle_access
            x: 95
            y: 319
            width: 228
            height: 33
            color: "#D9D9D9"

            Text {
                id: access
                x: 5
                y: 0
                width: 218
                height: 33
                text: qsTr("Accès")
                color: "#171F69"
                font.styleName: "Bold"
                font.pixelSize: 14
                verticalAlignment: Text.AlignVCenter
            }
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
            x: 130
            y: 372
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
    }

    states: [
        State {
            name: "clicked"
        }
    ]
}
