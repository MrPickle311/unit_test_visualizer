import QtQuick 2.15
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "../common" as Common

Common.FramelessWindow{
    id: terminalWindow
    maximumHeight: 600
    maximumWidth: 800
    minimumHeight: 600
    minimumWidth: 800

    TabBar {
        z: 2
        id: bar
        width: parent.width - terminalWindow.closeButton.width - 5
        anchors.verticalCenter: terminalWindow.closeButton.verticalCenter

        TerminalTabButton {
            text: qsTr("COM1")
        }
        TerminalTabButton {
            text: qsTr("COM2")
        }
        TerminalTabButton {
            text: qsTr("COM3")
        }
    }

    /*
      TODO: extract a one SINGLE TabButton to .qml file ,
            ,,--"         TextArea to .qml file

     */

    property list<TerminalPage> pages: [
            TerminalPage{ },
            TerminalPage{}
            //erminalPage{color: "green"; visible: true}
    ]

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
        for(var i = 0 ; i < pages.length; i++)
            swipeView.addItem(pages[i])
    }

}
