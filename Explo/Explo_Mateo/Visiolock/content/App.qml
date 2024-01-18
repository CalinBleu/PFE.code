// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick 6.2
import QtQuick.VirtualKeyboard 6.2
import QtQuick.Controls
import Visiolock


Window {
    width: Constants.width
    height: Constants.height

    visible: true
    title: "Visiolock"

    //visibility: Window.FullScreen

    InputPanel {
        id: inputPanel
        x: 0
        y: 0
        width: Constants.width
        height: Constants.height

        StackView {
            id: stack
            initialItem: AccueilAdmin {}
            anchors.fill: parent
        }
    }
}

