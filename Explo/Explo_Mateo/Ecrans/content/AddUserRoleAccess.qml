import QtQuick 6.5

AddUserRoleAccessForm {
    button_validate.onClicked: {
        stack.push("AddUserResume.qml")
    }
    button_cancel.onClicked: {
        stack.clear()
        stack.push("ParametersAdmin.qml")
    }

    role_input.onAccepted: {
        if (find(currentText) === -1) {
            model.append({text: editText})
            currentIndex = find(editText)
        }
    }
}
