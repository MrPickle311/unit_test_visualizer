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
        flow: Grid.LeftToRight
        anchors.fill: parent
        columns: 2
        rows: 2
        spacing: 50

        topPadding: mainWindow.height * 0.1
        leftPadding: mainWindow.width * 0.2

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
            z: 1
        }


    }


    MainWindow.MainWindowButton{
        id : closeButton
        iconDir: "qrc:/data/main_window/close.png"
        width: 32
        height: 32
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.rightMargin: 5
        onClicked: Qt.quit()
    }


    //TODO: frameless window
    MouseArea{
        id : mainMenuMouseArea
        anchors.fill: parent

        property variant clickPos: "1,1"

        onPressed:  MainWindowLogic.getClickPos(mainMenuMouseArea)

        onPositionChanged: MainWindowLogic.updateWindowPos(mainMenuMouseArea, mainWindow)
    }
}


/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:0.8999999761581421;height:480;width:640}
}
##^##*/
