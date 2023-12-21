import QtQuick 6.5
import Visio_v2

ModifyUserResumeForm {

    button_photo.onClicked: {
        stack.push("ModifyUserPicture.qml")
    }
    button_validate.onClicked: {
        stack.clear()
        stack.push("ParametersAdmin.qml")
    }
    button_cancel.onClicked: {
        stack.clear()
        stack.push("ParametersAdmin.qml")
    }
}
