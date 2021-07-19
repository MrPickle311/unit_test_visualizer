import QtQuick 2.15
import QtQuick.Window 2.15
import "framelessWindowLogic.js" as FramelessWindowLogic

Window {

    id: framelessWindow
    property bool isSizeConst: false//if its true maxmimum == minimum

    minimumWidth: 400
    minimumHeight: 400

    maximumHeight: isSizeConst ? minimumHeight : 500
    maximumWidth: isSizeConst ? minimumWidth : 500

    visible: true
    color: "white"
    flags: Qt.Window | Qt.CustomizeWindowHint

    MouseArea{
        id : framelessWindowMouseArea
        anchors.fill: parent
        z: 1
        property variant clickPos: "1,1"
        onPressed:  FramelessWindowLogic.getClickPos(framelessWindowMouseArea)
        onPositionChanged: FramelessWindowLogic.updateWindowPos(framelessWindowMouseArea, framelessWindow)
    }

    property variant closeButton: closeButton//to put closeButton outside

    ExitButton{
        id : closeButton
        onClicked: framelessWindow.close()
    }

}
