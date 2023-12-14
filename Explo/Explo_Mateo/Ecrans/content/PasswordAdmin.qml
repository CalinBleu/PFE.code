import QtQuick 6.5

PasswordAdminForm {
    button_password.onClicked: {
        stack.push("ParametersAdmin.qml")
    }
    button_quit.onClicked: {
        stack.clear()
        stack.push("AccueilAdmin.qml")
    }
}
