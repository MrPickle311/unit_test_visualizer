import QtQuick 2.15
import QtQuick.Controls 2.15
import "../common" as Common

Rectangle {
    id: rectangle
    color: "white"
    property int comNumber: 0

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

    BaudSlider{
        id: baudSlider
        anchors.top: parent.top
        anchors.horizontalCenterOffset: -60
        anchors.topMargin: 80
        anchors.horizontalCenter: parent.horizontalCenter
    }

   Common.MenuComboBox{
       id: parityComboBox
       anchors.top: baudSlider.bottom
       anchors.horizontalCenterOffset: 10
       anchors.topMargin: 50
       anchors.horizontalCenter: baudSlider.horizontalCenter

       prefixText: "Parity"
       elements: ["None","Odd","Even","Space","Mark"]
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
   }


   Common.MenuComboBox{
       id: displayComboBox
       anchors.right: stopBitsComboBox.right
       anchors.top: stopBitsComboBox.bottom
       anchors.rightMargin: 0
       anchors.horizontalCenterOffset: 0
       anchors.topMargin: 50
       anchors.horizontalCenter: stopBitsComboBox.horizontalCenter

       prefixText: "Display as"
       elements: ["Ascii","uint8","int8","Hexadecimal","Binary"]
   }

   Text {
       id: text1
       x: 310
       y: 395
       text: qsTr("FONT HERE!!!")
       font.pixelSize: 12
   }




}
