
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

        MediaDevices {
            id: mediaDevices
        }

        CaptureSession {
            camera: Camera {
                id: camera
                cameraDevice: mediaDevices.defaultVideoInput
                focusMode: Camera.FocusModeAutoNear
                customFocusPoint: Qt.point(0.2, 0.2)
                active: true
            }
            videoOutput: videoOutput
        }

        VideoOutput {
            id: videoOutput
            x:240
            y:131
            width: 320
            height: 259
            fillMode: VideoOutput.PreserveAspectCrop

            Rectangle {
                id: rectangle1
                x: -4
                y: -4
                width: 328
                height: 267
                color: "transparent"
                radius: 17
                border.color: "#171F69"
                border.width: 4

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
            y: 416
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
