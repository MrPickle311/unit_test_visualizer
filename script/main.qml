import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import "common" as Common
import "MainWindowLogic.js" as MainWindowLogic
import "settings_window/settingsWindow.js" as SettingsWindowLogic
import "terminal/terminalWindow.js" as TerminalWindowLogic
import "tests/testWindowLogic.js" as TestWindowLogic
import QtQuick.Dialogs 1.2

import Qt.singletons.bridge 1.0

Common.FramelessWindow {
    id: mainWindow
    title: "UartVisualizer"

    isSizeConst: true

    property var isWindowCreated: [false ,false ,false ,false]

    property var settingsWindowHandler: undefined
    property var terminalWindowHandler: undefined
    property var testWindowHandler: undefined

    function markWindowNotCreated(window_id){
        console.log(window_id)
        console.log(isWindowCreated)
        isWindowCreated[windowId] = false
        console.log(isWindowCreated)
    }

    //ERROR SERVICE!!!
    //Component.onCompleted: ErrorReporter.propagateError.connect(logError)

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
            onClicked: {
                if(!isWindowCreated[0])
                {
                    settingsWindowHandler = SettingsWindowLogic.createSettignsWindow()

                    settingsWindowHandler.windowId  = 0

                    isWindowCreated[0] = true

                    settingsWindowHandler.closing.connect(markWindowNotCreated)
                }
            }
        }

        Common.MenuButton{
            id: terminalButton
            iconDir: "qrc:/data/main_window/terminal.png"
            onClicked: {
                if(!isWindowCreated[1])
                {
                    terminalWindowHandler = TerminalWindowLogic.createTerminalWindow()

                    terminalWindowHandler.windowId  = 1

                    isWindowCreated[1] = true

                    terminalWindowHandler.closing.connect(markWindowNotCreated)
                }
            }
        }

        Common.MenuButton{
            id : testButton
            iconDir: "qrc:/data/main_window/test.png"
            onClicked: {
                console.log(isWindowCreated[2])
                if(!isWindowCreated[2])
                {
                    testWindowHandler = TestWindowLogic.createTestsWindow()

                    testWindowHandler.windowId  = 2

                    isWindowCreated[2] = true

                    testWindowHandler.closing.connect(markWindowNotCreated)
                }

            }
        }

        Common.MenuButton{
            id : aboutButton
            iconDir: "qrc:/data/main_window/about.png"

        }
    }

    MessageDialog {
        id: msgBox
        title: "Overwrite?"
        icon: StandardIcon.NoIcon
        text: "Error has occurred!"
        //detailedText: "To replace a file means that its existing contents will be lost. " +
        //    "The file that you are copying now will be copied over it instead."
        standardButtons: StandardButton.Ok
    }

    function showError(msg){
        console.log("XDXDXD")
        msgBox.text = msg
        msgBox.open()
    }

    Component.onCompleted: ErrorReporter.propagateError.connect(showError)



}
