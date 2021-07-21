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

    function createPage(port_name){
        var page = GlobalFUnctions.createComponent("qrc:/script/terminal/TerminalPage.qml")
        swipeView.addItem(page)
        page.portName = port_name
        page.openPortRequest.connect(TerminalBridge.openPort)
        listModel.append({page})
    }

    function addPortPage(port_name){
        pagesId[port_name] = swipeView.count
        createPage(port_name)
        var array = Object.keys(pagesId) //repeater does not refresh itself automatically
        repeater.model = array
    }

    function tryRestore(){
        var port_names = TerminalBridge.restorePorts()
        repeater.model = port_names
        for(var i = 0 ; i < port_names.length ; i++){
            pagesId.set(port_names[i] , swipeView.count )
            createPage(port_names[i])
        }
    }

    function receiceData(port_name , data){
        swipeView.itemAt(pagesId[port_name]).appendTextToOutput(data)
    }

    ListModel{
        id : listModel
    }

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

        onCurrentIndexChanged: swipeView.setCurrentIndex(bar.currentIndex)

    }

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

        //currentIndex: bar.currentIndex

        onCurrentIndexChanged: console.log(currentIndex + " items : " + listModel.count)
    }

    Component.onCompleted: {
        TerminalBridge.newPortIsSet.connect(addPortPage)
        TerminalBridge.dataArrived.connect(receiceData)
        tryRestore()
    }

    onClosing: TerminalBridge.closeAllPorts()

}
