import QtQuick 2.15
import QtQuick.Controls 2.12
import "../common" as Common

Common.FrameRectangle {
    id: textAreaRectangle

    TextArea{
        id: textArea
        anchors.fill: parent
        readOnly: true
        text: textAreaRectangle.text
    }
    property string text: ""

    function appendText(new_text){
        textArea.text = textArea.text.concat(new_text)
    }
}
