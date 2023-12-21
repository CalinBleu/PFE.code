// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick 6.2
import QtQuick.VirtualKeyboard 6.2
import QtQuick.Controls
import Visio_v2


Window {
    width: Constants.width
    height: Constants.height

    visible: true
    title: "Visiolock"

    visibility: Window.FullScreen

    StackView {
        id: stack
        initialItem: AddUserPicture {}
        anchors.fill: parent
    }

}

