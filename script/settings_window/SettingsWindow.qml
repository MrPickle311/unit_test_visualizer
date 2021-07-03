import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15


Window {
    id: settingsWindow
    height: 400
    width: 400
    visible: true

    MenuBar{
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.leftMargin: 0

        Menu{
            title: "Options for..."
            width: 80
            Action{

                text: "Terminal"
            }
            Action{
                text: "Test machine"
            }
        }

    }
}

