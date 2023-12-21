import QtQuick 6.5
import Visio_v2

AddUserPictureForm {
    button_validate.onClicked: {
        stack.push("AddUserNameFirstname.qml")
    }
    button_cancel.onClicked: {
        stack.clear()
        stack.push("ParametersAdmin.qml")
    }
}
