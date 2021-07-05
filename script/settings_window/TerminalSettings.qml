import QtQuick 2.15
import QtQuick.Controls 2.15
import "../common" as Common

Rectangle {
    color: "white"

    Common.RefreshButton{
        x: 113
        y: 83
        onClicked: console.log("xd")
    }

    Button{
        text: "ok"
        onClicked: console.log("xd")
    }
}
