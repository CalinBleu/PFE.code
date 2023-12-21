import QtQuick 6.5
import Visio_v2

AddUserRFIDForm {
    button_validate.onClicked: {
        stack.push("AddUserPicture.qml")
    }
    button_cancel.onClicked: {
        stack.clear()
        stack.push("ParametersAdmin.qml")
    }
}
