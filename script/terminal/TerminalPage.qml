import QtQuick 2.15
import QtQuick.Controls 2.12
import "../common" as Common

Rectangle {
    id: pageRectangle
    color: "white"

    property string portName: "none"

    signal openPortRequest()

    TerminalTextArea{
        id: outputTextArea
        width: ( parent.width - anchors.leftMargin.valueOf() * 3 ) / 2//every margin has 20 ,so i just taken only one and multiplied by 3
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: clearOutputButton.top
        anchors.bottomMargin: 10
        anchors.leftMargin: 20
        anchors.topMargin: 20
    }

    TerminalTextArea{
        id: inputTextArea
        width: outputTextArea.width
        height: outputTextArea.height
        anchors.left: outputTextArea.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 20
        anchors.leftMargin: 20
        anchors.topMargin: 20
    }

    function appendTextToOutput(data){
        outputTextArea.appendText(data)
    }

    function replaceOutputText(data){
        outputTextArea.setText(data)
    }

    TextStreamField{
        id : textStreamField
        y: 375
        width: 164
        height: 27
        anchors.left: parent.left
        anchors.bottom: sendModeComboBox.top
        anchors.bottomMargin: 20
        anchors.leftMargin: 20
    }

    SendButton{
        id: sendButton
        y: 326
        width: 32
        height: 32
        anchors.verticalCenter: textStreamField.verticalCenter
        anchors.left: textStreamField.right
        anchors.leftMargin: 20

        onClicked: inputTextArea.appendText(textStreamField.getText())
    }

    Common.MenuComboBox{
        id: sendModeComboBox
        y: 50
        anchors.verticalCenter: checkBoxLF.verticalCenter
        anchors.left: parent.left
        anchors.verticalCenterOffset: -25
        anchors.leftMargin: 100
        prefixText: "Sending mode"
        elements: ["Send Ascii", "Send Number"]
    }

    CheckBox {
        id: checkBoxCR
        x: 245
        y: 373
        text: qsTr("Apend +CR")
        anchors.bottom: checkBoxLF.top
        anchors.horizontalCenter: checkBoxLF.horizontalCenter
        anchors.bottomMargin: 20
    }

    CheckBox {
        id: checkBoxLF
        y: 401
        width: 102
        text: qsTr("Apend +LF")
        anchors.left: sendModeComboBox.right
        anchors.bottom: parent.bottom
        anchors.leftMargin: 150
        anchors.bottomMargin: 20
    }

    Common.MenuTextButton{
        id: clearOutputButton
        x: 258
        y: 322
        height: 32
        anchors.right: outputTextArea.right
        anchors.bottom: checkBoxCR.top
        anchors.bottomMargin: 30
        anchors.rightMargin: 0
        buttonText: "Clear output"
    }

    Common.MenuTextButton{
        id: freezeOutputButton
        x: 179
        y: 322
        anchors.verticalCenter: clearOutputButton.verticalCenter
        anchors.right: clearOutputButton.left
        anchors.rightMargin: 10
        buttonText: "Freeze output"
    }

    Common.MenuTextButton{
        id: clearInputButton
        y: 322
        anchors.verticalCenter: clearOutputButton.verticalCenter
        anchors.right: inputTextArea.right
        anchors.rightMargin: 0
        buttonText: "Clear input"
    }

    Common.MenuTextButton{
        id: chooseFileButton
        x: 544
        y: 393
        anchors.right: parent.right
        anchors.bottom: sendFileButton.bottom
        anchors.bottomMargin: 35
        anchors.rightMargin: 40
        buttonText: "Choose file..."
    }

    Common.MenuTextButton{
        id: openPortButton
        x: 101
        y: 298
        anchors.verticalCenter: freezeOutputButton.verticalCenter
        anchors.right: freezeOutputButton.left
        anchors.rightMargin: 10
        buttonText: "Open port"

        onClicked: openPortRequest()
    }

    Common.MenuTextButton{
        id: sendFileButton
        y: 428
        anchors.left: fileTextField.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: 0
        anchors.bottomMargin: 40
        buttonText: "Send file"
    }

    TerminalTextArea{
        id : fileTextField
        x: 367
        y: 395
        width: 164
        height: 27
        anchors.verticalCenter: chooseFileButton.verticalCenter
        anchors.right: chooseFileButton.left
        anchors.rightMargin: 20

    }
}
