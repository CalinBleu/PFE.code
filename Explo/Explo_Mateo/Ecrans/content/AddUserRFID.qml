import QtQuick 6.5

AddUserRFIDForm {
    button_validate.onClicked: {
        stack.push("AddUserPicture.qml")
    }
    button_cancel.onClicked: {
        stack.clear()
        stack.push("ParametersAdmin.qml")
    }
}
