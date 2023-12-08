
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

    property alias button_add: admin_add
    property alias button_suppr: admin_suppr
    property alias button_modify: admin_modify
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

        Rectangle {
            id: users
            x: 30
            y: 63
            width: 572
            height: 388
            color: "#ffffff"
            radius: 17
            border.color: "#171F69"
            border.width: 4

            Rectangle {
                id: search
                x: 22
                y: 13
                width: 342
                height: 22
                color: "#ffffff"
                border.color: "#171F69"
                border.width: 2
                radius: 8

                TextInput {
                    id: textInput
                    x: 15
                    y: 4
                    width: 291
                    height: 14
                    text: qsTr("Rechercher Utilisateur")
                    font.pixelSize: 12
                    color: "#171F69"
                    font.italic: true
                }

                Image {
                    id: image_search
                    x: 316
                    y: 1
                    width: 20
                    height: 20
                    source: "../content/images/search.png"
                    fillMode: Image.PreserveAspectFit
                }
            }

            ScrollView {
                id: scrollView
                x: 25
                y: 41
                width: 539
                height: 339
                clip: true

                GridView {
                    id: gridView
                    x: 0
                    y: 0
                    width: 140
                    height: 140
                    cellWidth: 178
                    model: ListModel {
                        ListElement {
                            name: "Grey"
                        }

                        ListElement {
                            name: "Red"
                        }

                        ListElement {
                            name: "Blue"
                        }

                        ListElement {
                            name: "siu"
                        }
                    }
                    delegate: Item {
                        x: 5
                        width: 159
                        height: 223
                        Column {
                            spacing: 5
                            Rectangle {
                                width: 159
                                height: 223
                                color: "#ffffff"
                                anchors.horizontalCenter: parent.horizontalCenter
                                radius: 17
                                border.width: 2
                                border.color: "#171F69"

                                Rectangle {
                                    y: 6
                                    width: 148
                                    height: 175
                                    color: "#ffffff"
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    radius: 17
                                    border.width: 1
                                    border.color: "#171F69"

                                    Text {
                                        text: qsTr("Photo")
                                        color: "#000000"
                                        font.pixelSize: 11
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        anchors.verticalCenter: parent.verticalCenter
                                    }
                                }

                                Text {
                                    x: 22
                                    y: 192
                                    text: qsTr("Nom")
                                    color: "#000000"
                                    font.pixelSize: 11
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }

                                Text {
                                    x: 82
                                    y: 192
                                    text: qsTr("Prénom")
                                    color: "#000000"
                                    font.pixelSize: 11
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }
                            }
                        }
                    }
                    cellHeight: 241
                }
            }
        }

        Button {
            id: admin_add
            x: 614
            y: 63
            width: 167
            height: 48
            contentItem: Text {
                text: qsTr("Ajouter utilisateur")
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
            id: admin_suppr
            x: 614
            y: 118
            width: 167
            height: 48
            contentItem: Text {
                text: qsTr("Supprimer utilisateur")
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
            id: admin_modify
            x: 614
            y: 173
            width: 167
            height: 48
            contentItem: Text {
                text: qsTr("Modifier utilisateur")
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
    }

    states: [
        State {
            name: "clicked"
        }
    ]
}
