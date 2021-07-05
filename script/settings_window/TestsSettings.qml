import QtQuick 2.15
import QtQuick.Controls 2.15
import "../common" as Common

Rectangle {
    color: "white"

    BaudSlider{
        id: baudSlider
        anchors.top: parent.top
        anchors.horizontalCenterOffset: -60
        anchors.topMargin: 80
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Common.ApplyButton{
        id: applyButton
        anchors.verticalCenter: refreshButton.verticalCenter
        anchors.right: refreshButton.left
        anchors.rightMargin: 20
    }

    Common.RefreshButton{
        id: refreshButton
        x: 154
        y: 54
        anchors.verticalCenter: comPortComboBox.verticalCenter
        anchors.right: comPortComboBox.left
        anchors.rightMargin: 20
    }

    ComboBox {
        id: comPortComboBox
        x: 215
        anchors.top: parent.top
        anchors.horizontalCenterOffset: 60
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 10

        //here C++ injects every found COM
        model: ListModel{
            id: comPortListModel
            ListElement{name: "COM"}
        }
    }


}
