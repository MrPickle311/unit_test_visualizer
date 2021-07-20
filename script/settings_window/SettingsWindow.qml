import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import "../common" as Common


Common.FramelessWindow{

    id: settingsWindow

    function prnt(){
        console.log("hello")
    }

    objectName: "settingsWindow"
    visible: true
    isSizeConst: true

    minimumWidth: 400
    minimumHeight: 500

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
                onTriggered: swipeView.setCurrentIndex(0)
            }
            MenuSeparator { }
            MenuItem {
                text: "Tests settings"
                onTriggered: swipeView.setCurrentIndex(1)

            }

            onOpened: choiceButton.blockRotation()
            onClosed: {
                choiceButton.unblockRotation()
                choiceButton.rotation = 0
            }
    }

    signal applyButtonClicked(string port_name)


    SwipeView{
        id: swipeView
        interactive: false
        currentIndex: 0
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: choiceButton.bottom
        anchors.bottom: parent.bottom
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        anchors.bottomMargin: 0
        anchors.topMargin: 0
        z: 2

        TerminalSettings{
            id: terminalSettings
            onApplyButtonClicked: settingsWindow.applyButtonClicked(port_name)
        }

        TestsSettings{
            id: testsSettings
        }

    }
}
