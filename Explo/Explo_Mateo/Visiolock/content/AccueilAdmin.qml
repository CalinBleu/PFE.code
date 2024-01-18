import QtQuick 6.5
import Visiolock
import QtQuick.VirtualKeyboard 6.2
import QtMultimedia

AccueilAdminForm {
    button.onClicked: {
        camera.stop()
        stack.push("PasswordAdmin.qml")
    }
}
