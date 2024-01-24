import QtQuick 6.5
import QtQuick.VirtualKeyboard 6.2
import Visiolock

PasswordAdminForm {
    button_password.onClicked: {
        stack.push("ParametersAdmin.qml")
    }
    button_quit.onClicked: {
        stack.clear()

        stack.push("AccueilAdmin.qml")
    }
    label_password.visible: {
        if (password_input.text.length === 0) {
            true
        } else {
            false
        }
    }
}
