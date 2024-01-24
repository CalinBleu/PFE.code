// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick 6.5
import QtQuick.VirtualKeyboard 6.2
import QtQuick.Controls
import Visiolock
import GuiModel


Window {
    width: Constants.width
    height: Constants.height

    visible: true
    title: "Visiolock"

    visibility: Window.FullScreen

    GuiModel{
        id: gui
    }

    Component.onCompleted: gui.Gui_start

      StackView {
        id: stack
        initialItem: AccueilAdmin {}
        anchors.fill: parent
    }


    InputPanel {
        id: inputPanel
        width: Constants.width
        height: Constants.height

        property bool showKeyboard :  active
        y: showKeyboard ? parent.height - height : parent.height
        Behavior on y {
            NumberAnimation {
                duration: 200
                easing.type: Easing.InOutQuad
            }
        }
    }
}

