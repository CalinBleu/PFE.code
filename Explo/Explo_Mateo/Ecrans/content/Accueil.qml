import QtQuick 6.5
import Ecrans
import QtMultimedia

AccueilForm {
    function stop() {
        camera.stop();
    }

    mouse.onClicked: camera.play()
}


