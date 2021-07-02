import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import "main_window" as MainWindow
import "MainWindowLogic.js" as MainWindowLogic


Window {
    id: mainWindow
    minimumWidth: 400
    minimumHeight: 400
    maximumHeight: 400
    maximumWidth: 400
    visible: true
    color: "white"
    title: "UartVisualizer"
    flags: Qt.FramelessWindowHint

    Grid {

        id: mainMenuGrid
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: closeButton.bottom
        anchors.bottom: parent.bottom
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        anchors.bottomMargin: 0
        anchors.topMargin: 0
        flow: Grid.LeftToRight
        columns: 2
        rows: 2
        spacing: 64
        z: 2
        topPadding: ( mainMenuGrid.height - 2 * settingsButton.height - spacing ) / 2
        leftPadding: ( mainMenuGrid.width - 2 * settingsButton.width - spacing ) / 2

        MainWindow.MainWindowButton{
            id: settingsButton
            iconDir: "qrc:/data/main_window/settings.png"
        }

        MainWindow.MainWindowButton{
            id: terminalButton
            iconDir: "qrc:/data/main_window/terminal.png"
        }

        MainWindow.MainWindowButton{
            id : testButton
            iconDir: "qrc:/data/main_window/test.png"
        }

        MainWindow.MainWindowButton{
            id : aboutButton
            iconDir: "qrc:/data/main_window/about.png"
        }


    }

    //TODO: frameless window
    MouseArea{
        id : mainMenuMouseArea
        anchors.fill: parent
        z: 1
        property variant clickPos: "1,1"
        onPressed:  MainWindowLogic.getClickPos(mainMenuMouseArea)
        onPositionChanged: MainWindowLogic.updateWindowPos(mainMenuMouseArea, mainWindow)
    }


    MainWindow.MainWindowButton{
        id : closeButton
        iconDir: "qrc:/data/main_window/close.png"
        width: 32
        height: 32
        z: 2
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.rightMargin: 5
        onClicked: Qt.quit()
    }
}
