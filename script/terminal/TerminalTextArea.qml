import QtQuick 2.15
import QtQuick.Controls 2.12
import "../common" as Common
import QtQuick.Layouts 1.12

Common.FrameRectangle {
    id: textAreaRectangle

    ScrollView{
        id: scrollView
        anchors.fill: parent
        anchors.margins: 7
        TextArea{
            id: textArea
            readOnly: true
            antialiasing: true
            wrapMode: TextEdit.Wrap
        }
    }

    function appendText(new_text){
        textArea.insert(textArea.length, new_text)
    }

    function setText(txt){
        textArea.remove(0,textArea.length)
        textArea.insert(0, txt)
    }
}
