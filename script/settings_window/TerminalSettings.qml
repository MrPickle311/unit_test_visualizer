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
            ListElement{ key: "COM1" }
             ListElement{ key: "COM2" }
              ListElement{ key: "COM3" }
        }
    }

   BaudSlider{
       id: baudSlider

   }

   ComboBox{
       id: parityComboBox
   }

}
