import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import "../common" as Common

Common.FramelessWindow{
    id: settingsWindow

    Common.ChoiceButton{
        id : choiceButton
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.leftMargin: 5

        //onClicked:
    }
}
