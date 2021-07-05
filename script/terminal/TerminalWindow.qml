import QtQuick 2.15
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "../common" as Common

Common.FramelessWindow{

    maximumHeight: 600
    maximumWidth: 800
    minimumHeight: 600
    minimumWidth: 800

    TabBar {
        z: 2
        id: bar
        width: parent.width
        TabButton {
            text: qsTr("Home")
        }
        TabButton {
            text: qsTr("Discover")
        }
        TabButton {
            text: qsTr("Activity")
        }
    }

    TabButton {
        id: clone
        text: qsTr("Clone")
    }
    /*
      TODO: extract a one TabButton to .qml file ,
            ,,--"         TextArea to .qml file

     */
    StackLayout {
        width: parent.width
        currentIndex: bar.currentIndex
        Item {
            id: homeTab
        }
        Item {
            id: discoverTab
        }
        Item {
            id: activityTab
        }
    }

    Component.onCompleted: bar.addItem( clone )

}
