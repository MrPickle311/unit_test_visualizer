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

        onOpenPortRequest: {
            var port_names = Object.keys(pagesId)
            TerminalBridge.openPort( port_names[bar.currentIndex])
        }

        onSendToOutputRequest: {
            if(outputDataList.length != 0)
                Logic.sendData(typeof(data) == "string" ?  data : String.fromCharCode(...data) )

                var view_data = typeof(data) == "string" ?  data : data.map(String).toString() + '  '

                Logic.refreshOutput(view_data)

                outputDataList[bar.currentIndex] += view_data

                //incoming data is still array of bytes or string
        }

        onInputSendModeChanged: Logic.resetInput()
        onClearInputRequested: Logic.resetInput()

        onOutputDisplayModeChanged: Logic.resetOutput()
        onClearOutputRequested: Logic.resetOutput()

        onFreezeInputRequested: isInputFreezed[bar.currentIndex] = !isInputFreezed[bar.currentIndex]
    }

    Component.onCompleted: {
        TerminalBridge.newPortIsSet.connect(Logic.addPortPage)
        TerminalBridge.dataArrived.connect(Logic.receiceData)
        Logic.tryRestore()
    }

    onClosing: TerminalBridge.closeAllPorts()

}
