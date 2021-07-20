import QtQuick 2.15
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "../common" as Common

import "../common/globalFunctions.js" as GlobalFUnctions

import Qt.singletons.bridge 1.0

Common.FramelessWindow{
    id: terminalWindow
    maximumHeight: 800
    maximumWidth: 800
    minimumHeight: 400
    minimumWidth: 800

    function createPage(port_name){
        var page = GlobalFUnctions.createComponent("qrc:/script/terminal/TerminalPage.qml")
        page.portName = port_name
        swipeView.addItem(page)
    }

    function addPortPage(port_name){
        createPage(port_name)
        portNames.push(port_name)
        repeater.model = portNames//repeater does not refresh itself automatically
    }

    function tryRestore(){
        portNames = TerminalBridge.restorePorts()
        repeater.model = portNames
        for(var i = 0 ; i < portNames.length ; i++)
            createPage(portNames[i])
    }

    property var portNames: []

    TabBar {
        z: 2
        id: bar
        width: TerminalTabButton.width * bar.count
        anchors.verticalCenter: terminalWindow.closeButton.verticalCenter

        Repeater{
            id: repeater
            model: portNames

            TerminalTabButton {
                text: modelData
            }

            onItemAdded: console.log("added!")
        }
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

        currentIndex: bar.currentIndex
    }

    Component.onCompleted: {
        TerminalBridge.newPortIsSet.connect(addPortPage)
        tryRestore()


    }

}
