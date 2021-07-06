import QtQuick 2.15
import QtQuick.Controls 2.12
import "../common" as Common

Rectangle {
    id: rectangle
    color: "white"

    TerminalTextArea{
        id: outputTextArea
        x: 23
        y: 24
        width: 301
        height: 292
    }

    TerminalTextArea{
        id: inputTextArea
        x: 348
        y: 24
        width: 276
        height: 292
    }

    TextStreamField{
        id : textStreamField
        x: 23
        y: 375
        width: 164
        height: 27
    }

    SendButton{
        id: sendButton
        y: 326
        width: 32
        height: 32
        anchors.verticalCenter: textStreamField.verticalCenter
        anchors.left: textStreamField.right
        anchors.leftMargin: 20
    }

    Common.MenuComboBox{
        id: sendModeComboBox
        anchors.left: parent.left
        anchors.top: textStreamField.bottom
        anchors.leftMargin: 100
        anchors.topMargin: 20
        prefixText: "Sending mode"
        elements: ["Send Ascii", "Send Number"]
    }

    CheckBox {
        id: checkBoxCR
        x: 245
        y: 373
        text: qsTr("Apend +CR")
    }

    CheckBox {
        id: checkBoxLF
        x: 348
        width: 102
        text: qsTr("Apend +LF")
        anchors.top: checkBoxCR.bottom
        anchors.horizontalCenter: checkBoxCR.horizontalCenter
        anchors.topMargin: 10
    }

    Common.MenuTextButton{
        x: 258
        y: 322
        buttonText: "Clear output"
    }

    Common.MenuTextButton{
        x: 179
        y: 322
        buttonText: "Freeze output"
    }

    Common.MenuTextButton{
        x: 565
        y: 322
        buttonText: "Clear input"
    }

    Common.MenuTextButton{
        x: 544
        y: 393
        buttonText: "Choose file..."
    }

    Common.MenuTextButton{
        x: 367
        y: 428
        buttonText: "Send file"
    }

    TerminalTextArea{
        id : fileTextField
        x: 367
        y: 395
        width: 164
        height: 27

    }
}
