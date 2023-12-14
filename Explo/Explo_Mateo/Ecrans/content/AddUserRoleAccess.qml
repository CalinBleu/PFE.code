import QtQuick 6.5

AddUserRoleAccessForm {
    button_validate.onClicked: {
        stack.push("AddUserResume.qml")
    }
    button_cancel.onClicked: {
        stack.clear()
        stack.push("ParametersAdmin.qml")
    }
}
