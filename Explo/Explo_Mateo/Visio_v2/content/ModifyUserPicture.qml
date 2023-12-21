import QtQuick 6.5
import Visio_v2

ModifyUserPictureForm {

    button_validate.onClicked: {
        stack.pop()
    }
    button_cancel.onClicked: {
        stack.clear()
        stack.push("ModifyUserResume.qml")
    }
}
