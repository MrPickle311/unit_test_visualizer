import QtQuick 2.15
import QtQuick.Controls 2.12
import "../common" as Common

Common.FrameRectangle {
    id: textAreaRectangle

    TextArea{
        anchors.fill: parent
        readOnly: true
    }
}
