import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import "main_window" as MainWindow
import QtQuick.Layouts 1.15

Window {
    id: mainWindow
    minimumWidth: 400
    minimumHeight: 400
    maximumHeight: 400
    maximumWidth: 400
    visible: true
    color: "white"
    title: "UartVisualizer"

    Grid {
        verticalItemAlignment: Grid.AlignVCenter
        horizontalItemAlignment: Grid.AlignHCenter
        flow: Grid.LeftToRight
        id: grid
        anchors.fill: parent
        columns: 2
        rows: 3
        spacing: 50

        topPadding: mainWindow.height * 0.2
        leftPadding: mainWindow.width * 0.2

        MainWindow.MainWindowButton{
            id: btn
        }

        Button {
            id: button
            text: qsTr("Button")
        }

        Button {
            id: button1
            text: qsTr("Button")
        }

        Button {




            id: button4
            text: qsTr("Button")
        }





    }



}



/*##^##
Designer {
    D{i:0;formeditorZoom:0.6600000262260437}
}
##^##*/
