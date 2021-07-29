import QtQuick 2.15
import QtQuick.Controls 2.15
import "../common" as Common

import "settingsWindow.js" as Logic

import Qt.singletons.bridge 1.0

Rectangle {
    id: rectangle
    color: "white"

    signal applyButtonClicked(string port_name)

    Common.ApplyButton{
        id: applyButton
        anchors.verticalCenter: refreshButton.verticalCenter
        anchors.right: refreshButton.left
        anchors.rightMargin: 20

        onClicked: TerminalSettingsBridge.sendSettings(comPortComboBox.currentValue)
    }

    Common.RefreshButton{
        id: refreshButton
        x: 154
        y: 54
        anchors.verticalCenter: comPortComboBox.verticalCenter
        anchors.right: comPortComboBox.left
        anchors.rightMargin: 20

        onClicked: Scanner.scanPorts()
    }


    ComboBox {
        id: comPortComboBox
        x: 215
        anchors.top: parent.top
        anchors.horizontalCenterOffset: 60
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 10

        //here C++ injects each found COM
        model: TerminalSettingsBridge.portNames
    }

    BaudSlider{
        id: baudSlider
        anchors.top: parent.top
        anchors.horizontalCenterOffset: -60
        anchors.topMargin: 80
        anchors.horizontalCenter: parent.horizontalCenter

        onPositionChanged: TerminalSettingsBridge.setBaudRate(Logic.bauds.get(value))
    }

   Common.MenuComboBox{
       id: parityComboBox
       anchors.top: baudSlider.bottom
       anchors.horizontalCenterOffset: 10
       anchors.topMargin: 50
       anchors.horizontalCenter: baudSlider.horizontalCenter

       prefixText: "Parity"
       elements: ["None","Odd","Even","Space","Mark"]

       body.onActivated: TerminalSettingsBridge.setParity(Logic.parity.get(elements[body.currentIndex]))
   }

   Common.MenuComboBox{
       id: dataBitsComboBox
       anchors.right: parityComboBox.right
       anchors.top: parityComboBox.bottom
       anchors.rightMargin: 0
       anchors.topMargin: 50
       anchors.horizontalCenter: parityComboBox.horizontalCenter

       prefixText: "Data bits"
       elements: ["5","6","7","8"]

       body.onActivated: TerminalSettingsBridge.setDataBits(Logic.dataBits.get(elements[body.currentIndex]))
   }

   Common.MenuComboBox{
       id: stopBitsComboBox
       anchors.right: dataBitsComboBox.right
       anchors.top: dataBitsComboBox.bottom
       anchors.rightMargin: 0
       anchors.topMargin: 50
       anchors.horizontalCenter: dataBitsComboBox.horizontalCenter

       prefixText: "Stop bits"
       elements: ["1","1.5","2"]

       body.onActivated: TerminalSettingsBridge.setStopBits(Logic.stopBits.get(elements[body.currentIndex]))
   }

   Common.MenuComboBox{
       id: flowControlComboBox
       anchors.right: stopBitsComboBox.right
       anchors.top: stopBitsComboBox.bottom
       anchors.rightMargin: 0
       anchors.topMargin: 50
       anchors.horizontalCenter: stopBitsComboBox.horizontalCenter

       prefixText: "Flow control"
       elements: ["No flow control","Hardware (RTS/CTS)","Software (XON/XOFF)"]

       //body.onActivated: TerminalSettingsBridge.setStopBits(Logic.stopBits.get(elements[body.currentIndex]))
   }
}
