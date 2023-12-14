import QtQuick 6.5

ModifyUserPictureForm {

    button_validate.onClicked: {
        stack.pop()
    }
    button_cancel.onClicked: {
        stack.clear()
        stack.push("ModifyUserResume.qml")
    }
}
