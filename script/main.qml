import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import "main_window" as MainWindow

ApplicationWindow {
    id: mainWindow
    width: 640
    height: 480
    visible: true
    color: "#ececec"
    title: "UartVisualizer"

    menuBar: MenuBar{
        id : menuBar
        height: 20
        contentHeight: 20
        Menu{
            id : mainMenu
            title: "File"
            //Rectangle.anchors.bottom: menuBar.bottom
           // height: menuBar.height
            //contentHeight: menuBar.height
            MenuItem {
                id : menuItem
                text : "New"
                height: menuBar.height
                onTriggered: {
                    console.log("got it xd")
                }
                Action{
                    shortcut: "Ctrl+E"
                    enabled: false
                    onTriggered: menuItem.triggered()
                }
            }
            MenuSeparator{}
            MenuItem{
                height: menuBar.height
                text: "Back"
            }
            MenuSeparator{}
            MenuItem{
                height: menuBar.height
                text: "Quit"
            }
        }
/*
        Menu{
            id : menu2
            title: "Edit"
            height: parent.height

            Action {
                text : "Cut"
            }

            MenuSeparator{}
        }

*/
    }


}


