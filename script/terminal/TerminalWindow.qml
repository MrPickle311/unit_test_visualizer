import QtQuick 2.15
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "../common" as Common

import "../common/globalFunctions.js" as GlobalFUnctions

import Qt.singletons.bridge 1.0

import "terminalWindow.js" as Logic

Common.FramelessWindow{
    id: terminalWindow
    maximumHeight: 800
    maximumWidth: 800
    minimumHeight: 400
    minimumWidth: 800

    property var outputDataList: []

    property var inputDataList: []

    property var pagesId: new Map();

    property var bridge: TerminalBridge

    //this array holds free-unfreeze state of certain port
    property var isInputFreezed: []

    TabBar {
        z: 2
        id: bar
        width: TerminalTabButton.width * bar.count
        anchors.verticalCenter: terminalWindow.closeButton.verticalCenter

        Repeater{
            id: repeater
            model: []

            TerminalTabButton {
                text: modelData
            }
        }

        onCurrentIndexChanged: Logic.changePortPage()
    }

    TerminalPage {
        id: terminalPage
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: terminalWindow.closeButton.bottom
        anchors.bottom: parent.bottom
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        anchors.bottomMargin: 0
        anchors.topMargin: 5
        z: 2

        onOpenPortRequest: Logic.openPort()

        onSendToOutputRequest: {//incoming data are still array of bytes or string
            if(outputDataList.length != 0)
            {
                //send to backend
                Logic.sendData(data)
                //update view
                console.log(data)
                Logic.refreshOutput(data)
            }
        }

        onInputSendModeChanged: Logic.resetInput()
        onClearInputRequested: Logic.resetInput()

        onFreezeInputRequested: isInputFreezed[bar.currentIndex] = !isInputFreezed[bar.currentIndex]

        onOutputDisplayModeChanged: Logic.resetOutput()
        onClearOutputRequested: Logic.resetOutput()
    }

    Component.onCompleted: {
        TerminalBridge.newPortIsSet.connect(Logic.addPortPage)
        TerminalBridge.dataArrived.connect(Logic.receiceData)
        Logic.tryRestore()
    }

    onClosing: TerminalBridge.closeAllPorts()
}
