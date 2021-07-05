import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import "../common" as Common

Common.FramelessWindow{
    id: settingsWindow

    Common.ChoiceButton{
        id : choiceButton
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.leftMargin: 5

        onClicked: contextMenu.popup(choiceButton.x , choiceButton.y + choiceButton.height)
    }

    Menu {
            id: contextMenu
            x: 5


            MenuItem { text: "Cut" }
            MenuItem { text: "Copy" }
            MenuItem { text: "Paste" }

            onOpened: choiceButton.blockRotation()
            onClosed: {
                choiceButton.unblockRotation()
                choiceButton.rotation = 0
            }
        }
}
