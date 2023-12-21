import QtQuick 6.5
import Visio_v2

ParametersAdminForm {

    button_add.onClicked: {
        stack.push("AddUserRFID.qml")
    }
    button_modify.onClicked: {
        stack.push("ModifyUserResume.qml")
    }
    button_quit.onClicked: {
        stack.clear()
        stack.push("AccueilAdmin.qml")
    }

    label_search.visible: {
        if (input_search.text.length === 0) {
            true
        } else {
            false
        }
    }
}
