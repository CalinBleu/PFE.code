import QtQuick 6.5

AddUserNameFirstnameForm {
    button_validate.onClicked: {
        stack.push("AddUserRoleAccess.qml")
    }
    button_cancel.onClicked: {
        stack.clear()
        stack.push("ParametersAdmin.qml")
    }
}
