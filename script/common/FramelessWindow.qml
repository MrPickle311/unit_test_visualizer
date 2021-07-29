import QtQuick 2.15
import QtQuick.Window 2.15
import "framelessWindowLogic.js" as FramelessWindowLogic

Window {
    objectName: "frameless"
    id: framelessWindow
    property bool isSizeConst: false//if its true maxmimum == minimum

    property var windowId: 0

    signal closing(int id)

    minimumWidth: 400
    minimumHeight: 400

    maximumHeight: isSizeConst ? minimumHeight : 500
    maximumWidth: isSizeConst ? minimumWidth : 500

    visible: true
    color: "white"
    flags: Qt.Window | Qt.CustomizeWindowHint

    MouseArea{
        objectName: "xd1"
        id : framelessWindowMouseArea
        anchors.fill: parent
        z: 1
        property variant clickPos: "1,1"
        onPressed:  FramelessWindowLogic.getClickPos(framelessWindowMouseArea)
        onPositionChanged: FramelessWindowLogic.updateWindowPos(framelessWindowMouseArea, framelessWindow)
    }

    property variant closeButton: closeButton//to put closeButton outside

    ExitButton{
        objectName: "xd2"
        id : closeButton
        onClicked: {
            closing(windowId)
            framelessWindow.close()
        }
    }

}
