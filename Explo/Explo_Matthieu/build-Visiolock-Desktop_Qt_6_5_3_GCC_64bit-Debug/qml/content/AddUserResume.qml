import QtQuick 6.5
import Visiolock

AddUserResumeForm {
    button_validate.onClicked: {
        stack.clear()
        stack.push("ParametersAdmin.qml")
    }
    button_cancel.onClicked: {
        stack.clear()
        stack.push("ParametersAdmin.qml")
    }
}
