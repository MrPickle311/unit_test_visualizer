import QtQuick 2.15
import QtQuick.Controls 2.15
import "../common" as Common

Rectangle {
    color: "white"
    property int comNumber: 0

    Common.RefreshButton{
        x: 113
        y: 83
    }

    ComboBox {
        id: comboBox
        x: 317
        y: 61

        //here C++ injects every found COM
        model: ListModel{
            id: comPortListModel
        }
    }

   BaudSlider{
       id: baudSlider
   }

   Common.MenuComboBox{
       id: parityComboBox
       prefixText: "Parity"
       elements: ["None","Odd","Even","Space","Mark"]
   }

   Common.MenuComboBox{
       id: dataBitsComboBox
       prefixText: "Data bits"
       elements: ["5","6","7","8"]
   }

   Common.MenuComboBox{
       id: stopBitsComboBox
       prefixText: "Stop bits"
       elements: ["1","1.5","2"]
   }


   Common.MenuComboBox{
       id: displayComboBox
       prefixText: "Display as"
       elements: ["Ascii","uint8","int8","Hexadecimal","Binary"]
   }



}
