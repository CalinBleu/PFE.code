

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
    id: it
    width: Constants.width
    height: Constants.height

    Rectangle {
        id: rectangle
        width: Constants.width
        height: Constants.height
        border.color: "#171F69"
        border.width: 4
        color: "white"

        Image {
            id: image2
            x: 342
            y: 25
            width: 117
            height: 15
            source: "../content/images/Thales 3.png"
            fillMode: Image.PreserveAspectFit
        }

        Image {
            id: image1
            x: 488
            y: -52
            width: 308
            height: 308
            source: "../content/images/Group 5.png"
            fillMode: Image.PreserveAspectFit
        }

        Image {
            id: image
            x: 4
            y: 114
            width: 372
            height: 362
            source: "../content/images/Group 6.png"
            fillMode: Image.PreserveAspectFit
        }

        Text {
            id: text1
            x: 207
            y: 71
            text: qsTr("Bienvenue, veuillez placer votre\n visage devant la camera")
            font.pixelSize: 26
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            transformOrigin: Item.Center
            font.styleName: "Bold"
            font.bold: true
            color: "#171F69"
        }

        Rectangle {
            id: rectangle1
            x: 217
            y: 133
            width: 328
            height: 267
            color: "#ffffff"
            radius: 17
            border.color: "#171F69"
            border.width: 4
            anchors.verticalCenterOffset: 47
            anchors.horizontalCenterOffset: 0
            anchors.centerIn: parent

            Image {
                id: corner1
                x: 73
                y: 39
                source: "../content/images/Group 1.png"
            }

            Image {
                id: corner2
                x: 225
                y: 39
                source: "../content/images/Group 1.png"
                rotation: 90
            }

            Image {
                id: corner3
                x: 225
                y: 194
                source: "../content/images/Group 1.png"
                rotation: 180
            }

            Image {
                id: corner4
                x: 73
                y: 194
                source: "../content/images/Group 1.png"
                rotation: 270
            }

            Video {
                id: video
                x: 0
                y: 0
                width: 328
                height: 267
            }
        }
    }

    states: [
        State {
            name: "clicked"
        }
    ]
}
