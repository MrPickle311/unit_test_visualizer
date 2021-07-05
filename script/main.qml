import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import "common" as Common
import "MainWindowLogic.js" as MainWindowLogic
import "settings_window/settingsWindow.js" as SettingsWindowLogic
import "terminal/terminalWindow.js" as TerminalWindowLogic

Common.FramelessWindow {

    id: mainWindow
    title: "UartVisualizer"

    isSizeConst: true

    Grid {

        id: mainMenuGrid
        property int buttonSize: 64
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top:  mainWindow.closeButton.bottom
        anchors.bottom: parent.bottom
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        anchors.bottomMargin: 0
        anchors.topMargin: 0
        flow: Grid.LeftToRight
        columns: 2
        rows: 2
        spacing: buttonSize
        z: 2
        topPadding: ( mainMenuGrid.height - 2 * settingsButton.height - spacing ) / 2
        leftPadding: ( mainMenuGrid.width - 2 * settingsButton.width - spacing ) / 2

        Common.MenuButton{
            id: settingsButton
            iconDir: "qrc:/data/main_window/settings.png"
            onClicked: SettingsWindowLogic.createSettignsWindow()
        }

        Common.MenuButton{
            id: terminalButton
            iconDir: "qrc:/data/main_window/terminal.png"
            onClicked: TerminalWindowLogic.createTerminalWindow()
        }

        Common.MenuButton{
            id : testButton
            iconDir: "qrc:/data/main_window/test.png"
        }

        Common.MenuButton{
            id : aboutButton
            iconDir: "qrc:/data/main_window/about.png"

        }

    }


}
