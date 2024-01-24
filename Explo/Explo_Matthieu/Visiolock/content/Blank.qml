import QtQuick 6.5


Item {



    Connections {
        target: GuiController
        onDisplayHomeScreenChanged: {
            if(GuiController.authResult === 0){
                stack.push("AccueilAdmin.qml");
            }
            else if(GuiController.authResult === 1){
                stack.push("Accueil.qml");
            }
            else if(GuiController.authResult === 2 || GuiController.authResult === 3){
                stack.push("AccueilResult.qml")
            }
        }
    }

}

