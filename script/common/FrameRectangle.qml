import QtQuick 2.15
import QtQuick.Controls 2.12

Rectangle {
    id: textRectangle
    property alias backgroundColor: textRectangle.color

    border.width: 1
    backgroundColor: "white"
    border.color: "black"
}
