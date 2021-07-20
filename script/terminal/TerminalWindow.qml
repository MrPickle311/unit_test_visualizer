import QtQuick 2.15
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "../common" as Common

import "../common/globalFunctions.js" as GlobalFUnctions

Common.FramelessWindow{
    id: terminalWindow
    maximumHeight: 800
    maximumWidth: 800
    minimumHeight: 400
    minimumWidth: 800

    TabBar {
        z: 2
        id: bar
        width: TerminalTabButton.width * bar.count
        anchors.verticalCenter: terminalWindow.closeButton.verticalCenter

        Repeater{

            model: ["COM1" , "COM4", "COMX"]

            TerminalTabButton {
                text: modelData
            }
        }
    }

    property var pages: []

    SwipeView {
        id: swipeView
        interactive: false
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: terminalWindow.closeButton.bottom
        anchors.bottom: parent.bottom
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        anchors.bottomMargin: 0
        anchors.topMargin: 5
        z: 2

        currentIndex: bar.currentIndex
    }

    Component.onCompleted: {

        pages.push(GlobalFUnctions.createComponent("qrc:/script/terminal/TerminalPage.qml"))

        for(var i = 0 ; i < pages.length; i++)
            swipeView.addItem(pages[i])
    }

}
