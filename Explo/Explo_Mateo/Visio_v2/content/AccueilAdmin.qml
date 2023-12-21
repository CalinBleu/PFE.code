import QtQuick 6.5
import Visio_v2
import QtMultimedia

AccueilAdminForm {
    button.onClicked: {
        stack.push("PasswordAdmin.qml")
    }
}
