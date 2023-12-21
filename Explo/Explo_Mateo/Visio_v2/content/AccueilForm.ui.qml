

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick 6.5
import QtMultimedia
import Visio_v2

Item {
    id: it
    width: Constants.width
    height: Constants.height

    property alias camera: camera

    Rectangle {
        id: rectangle
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
            id: messsage
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

        MediaDevices {
            id: mediaDevices
        }

        CaptureSession {
            camera: Camera {
                id: camera

                cameraDevice: mediaDevices.defaultVideoInput
                focusMode: Camera.FocusModeAutoNear
                active: true
            }
            videoOutput: videoOutput
        }

        VideoOutput {
            id: videoOutput
            x:222
            y:138
            width: 320
            height: 259
            fillMode: VideoOutput.PreserveAspectCrop
            anchors.verticalCenterOffset: 47
            anchors.horizontalCenterOffset: 0
            anchors.centerIn: parent

            Rectangle {
                id: rectangle_video
                x: -5
                y: -5
                width: 328
                height: 267
                radius: 17
                color: "transparent"
                border.color: "#171F69"
                border.width: 4
                anchors.centerIn: parent

                Image {
                    id: corner1
                    x: 73
                    y: 39
                    source: "../content/images/corner.png"
                }

                Image {
                    id: corner2
                    x: 225
                    y: 39
                    source: "../content/images/corner.png"
                    rotation: 90
                }

                Image {
                    id: corner3
                    x: 225
                    y: 194
                    source: "../content/images/corner.png"
                    rotation: 180
                }

                Image {
                    id: corner4
                    x: 73
                    y: 194
                    source: "../content/images/corner.png"
                    rotation: 270
                }
            }
        }


    }

    states: [
        State {
            name: "clicked"
        }
    ]
}
