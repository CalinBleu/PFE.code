// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick 6.5
import Ecrans
import QtQuick.VirtualKeyboard 6.5
import QtQuick.Controls

Window {
    width: Constants.width
    height: Constants.height

    visible: true
    title: "Ecrans"

    StackView {
        id: stack
        initialItem: AccueilAdmin {}
        anchors.fill: parent
    }

}

