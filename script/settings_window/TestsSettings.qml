import QtQuick 2.15
import QtQuick.Controls 2.15
import "../common" as Common

import Qt.singletons.bridge 1.0

import "settingsWindow.js" as SJ

Rectangle {
    color: "white"

    BaudSlider{
        id: baudSlider
        anchors.top: parent.top
        anchors.horizontalCenterOffset: -60
        anchors.topMargin: 80
        anchors.horizontalCenter: parent.horizontalCenter

        onPositionChanged: TestsSettingsBridge.setBaudRate(SJ.bauds.get(value))
    }

    Common.ApplyButton{
        id: applyButton
        anchors.verticalCenter: refreshButton.verticalCenter
        anchors.right: refreshButton.left
        anchors.rightMargin: 20

        onClicked: TestsSettingsBridge.sendSettings(comPortComboBox.currentValue)
    }

    Common.RefreshButton{
        id: refreshButton
        x: 154
        y: 54
        anchors.verticalCenter: comPortComboBox.verticalCenter
        anchors.right: comPortComboBox.left
        anchors.rightMargin: 20

        onClicked: TestsSettingsBridge.scanPorts()
    }

    ComboBox {
        id: comPortComboBox
        x: 215
        anchors.top: parent.top
        anchors.horizontalCenterOffset: 60
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 10

        //here C++ injects every found COM
        model: TestsSettingsBridge.portNames
    }
}
