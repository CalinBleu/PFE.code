import QtQuick 6.2
import Visiolock
import QtMultimedia 6.3

AccueilAdminForm {

    property int countdown: 20

    timer.onTriggered : {
        countdown -= 1;
        video.grabToImage(function(result) {
            result.saveToFile("/Users/mateorondeau/Documents/ESEO/PFE/Explorations/Test_On_QML/build-Visiolock-Qt_6_5_3_for_macOS-Debug/something.png")
         })
        if (countdown <= 0) {
            timer.stop();
            console.log("Compte à rebours terminé!");
        }
    }

    button.onClicked: {
        camera.stop();
        stack.push("PasswordAdmin.qml")
    }
}
