import QtQuick 2.15
import QtQuick.Controls 2.12
import "../common" as Common
import QtQuick.Layouts 1.12

Common.FrameRectangle{
    id: textInputRectangle

    TextField{
        anchors.fill: parent
        id: textInput
        readOnly: false
        antialiasing: true

        font.pixelSize: 16
    }

    function getText(){
        return textInput.text
    }
}
