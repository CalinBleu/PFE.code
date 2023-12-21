import QtQuick 6.5

AddUserNameFirstnameForm {
    button_validate.onClicked: {
        stack.push("AddUserRoleAccess.qml")
    }
    button_cancel.onClicked: {
        stack.clear()
        stack.push("ParametersAdmin.qml")
    }
    name_label.visible: {
        if (name_input.text.length === 0) {
            true
        } else {
            false
        }
    }
    firstname_label.visible: {
        if (firstname_input.text.length === 0) {
            true
        } else {
            false
        }
    }
}
