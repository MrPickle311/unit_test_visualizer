import QtQuick 2.15
import QtQuick.Controls 2.12

Item {
    id: item1

    property string prefixText: ""
    property var elements: []

    Text{
        id: prefix
        text: prefixText
        anchors.verticalCenter: comboBox.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: -1*prefix.paintedWidth - 5
    }

    ListModel{
        id: listModel
    }

    ComboBox{
        id: comboBox
        model: listModel
        currentIndex: 0


    }

    Component.onCompleted: {
        for (var i = 0; i < elements.length; i++)
            listModel.append({ name: elements[i] })
    }

    property alias body: comboBox

    property alias currentIndex: comboBox.currentIndex
}
