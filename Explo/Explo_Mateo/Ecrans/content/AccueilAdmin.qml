import QtQuick 6.5
import Ecrans
import QtMultimedia

AccueilAdminForm {
    button.onClicked: {
        stack.push("PasswordAdmin.qml")
    }
}
