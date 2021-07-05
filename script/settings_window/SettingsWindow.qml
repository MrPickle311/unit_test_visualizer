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

            width: 100
            MenuItem {
                text: "Terminal settings"
                onTriggered: swipeView.currentIndex = 0
            }
            MenuSeparator { }
            MenuItem {
                text: "Tests settings"
                onTriggered: swipeView.currentIndex = 1

            }

            onOpened: choiceButton.blockRotation()
            onClosed: {
                choiceButton.unblockRotation()
                choiceButton.rotation = 0
            }
        }


    SwipeView{
        id: swipeView
        interactive: false
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: choiceButton.bottom
        anchors.bottom: parent.bottom
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        anchors.bottomMargin: 0
        anchors.topMargin: 0

        Rectangle{
            id: rec1
            color: "blue"
        }

        Rectangle{
            id: rec2
            color: "red"
        }
    }
}
