import QtQuick 2.15
import QtQuick.Controls 2.12
import "../common" as Common

Common.FrameRectangle{
    id: textInputRectangle
    TextInput{
        id: textInput
        anchors.fill: parent
        readOnly: false
    }
    function getText(){
        return textInput.text
    }
}
