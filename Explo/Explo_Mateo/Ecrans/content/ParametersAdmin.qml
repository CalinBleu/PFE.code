import QtQuick 6.5

ParametersAdminForm {

    button_add.onClicked: {
        stack.push("AddUserRFID.qml")
    }
    button_modify.onClicked: {
        stack.push("ModifyUserResume.qml")
    }
    button_quit.onClicked: {
        stack.clear()
        stack.push("AccueilAdmin.qml")
    }
}
