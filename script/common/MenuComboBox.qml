import QtQuick 2.15
import QtQuick.Controls 2.12

Item {
    id: item1

    property string prefixText: ""
    property var elements: []

    Text{
        id: txt
        text: prefixText
        anchors.verticalCenter: comboBox.verticalCenter
        anchors.right: comboBox.left
        anchors.rightMargin: txt.paintedWidth
    }

    ListModel{
        id: listModel
    }

    ComboBox{
        id: comboBox
        x: 278
        y: 90
        model: listModel
    }

    Component.onCompleted: {
        for (var i = 0; i < elements.length; i++)
            listModel.append({ name: elements[i] })
    }

}
