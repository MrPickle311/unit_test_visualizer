import QtQuick 2.15
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "../common" as Common

import "../common/globalFunctions.js" as GlobalFUnctions

import Qt.singletons.bridge 1.0

import "terminalWindow.js" as TWLogic // TerminalWindowLogic

Common.FramelessWindow{
    id: terminalWindow
    maximumHeight: 800
    maximumWidth: 800
    minimumHeight: 400
    minimumWidth: 800

    function addPortPage(port_name){
        pagesId[port_name] = repeater.count
        outputDataTextList.push("")
        var array = Object.keys(pagesId) //repeater does not refresh itself automatically
        repeater.model = array
    }

    property int pagesCount: 0

    function tryRestore(){
        var port_names = TerminalBridge.restorePorts()
        for(var i = 0 ; i < port_names.length ; i++){
            pagesId[ port_names[i] ] = i
            outputDataTextList.push("")
        }
        repeater.model = port_names
    }

    function refresh(){
        terminalPage.replaceOutputText(outputDataTextList[bar.currentIndex])
    }

    function receiceData(port_name , data){
        outputDataTextList[pagesId[port_name]] += data
        console.log("arrived current data : " + bar.currentIndex + " " + pagesId[port_name])
        refresh()
    }

    property var outputDataTextList: []

    property var pagesId: new Map();

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

            onItemAdded: console.log("added!")

        }

        onCurrentIndexChanged: terminalPage.replaceOutputText(outputDataTextList[currentIndex])
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
            TerminalBridge.openPort(port_names[bar.currentIndex])
        }
    }

    Component.onCompleted: {
        TerminalBridge.newPortIsSet.connect(addPortPage)
        TerminalBridge.dataArrived.connect(receiceData)
        tryRestore()
    }

    onClosing: TerminalBridge.closeAllPorts()

}
