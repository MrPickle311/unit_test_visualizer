import QtQuick 2.15
import QtQuick.Controls 2.12
import "../common" as Common

import "terminalPage.js" as Logic

Rectangle {
    id: pageRectangle
    color: "white"
    clip: false

    property string portName: "none"

    signal openPortRequest()
    signal sendToOutputRequest(var data)

    signal outputDisplayModeChanged()
    signal inputSendModeChanged()

    signal clearInputRequested()
    signal clearOutputRequested()

    signal freezeInputRequested()

    height: 405
    width: 800

    property var inputConverters: [
        function(data) { return Logic.toAscii(data) },
        function(data) { return Logic.toNumbersString(data,10) },
        function(data) { return Logic.toNumbersString(data,16) },
        function(data) { return Logic.toNumbersString(data,2) }
    ]

    property var outputConverters: [
        function(str) { return str;},
        function(str) { return Logic.convertNmbrToBytes(str,1) },//uint8
        function(str) { return Logic.convertNmbrToBytes(str,2) },//uint16
        function(str) { return Logic.convertNmbrToBytes(str,4) },//uint32
        function(str) { return Logic.convertNmbrToBytes(str,8) }//uint64
    ]

    function appendTextToInput(data){
        inputTextArea.appendText(inputConverters[inputDisplayComboBox.currentIndex](data))
    }

    function replaceInputText(data){
        inputTextArea.setText(inputConverters[inputDisplayComboBox.currentIndex](data))
    }

    function resetInputTextArea(){
        inputTextArea.setText("")
    }

    function resetOutputTextArea(){
        outputTextArea.setText("")
    }

    function appendTextToOutput(data){
        outputTextArea.appendText(data)
    }

    function replaceOutputText(data){
        outputTextArea.setText(outputConverters[outputSendModeComboBox.currentIndex](data))
    }

    TerminalTextArea{
        id: inputTextArea
        width: ( parent.width - anchors.leftMargin.valueOf() * 3 ) / 2//every margin has 20 ,so i just taken only one and multiplied by 3
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: clearOutputButton.top
        anchors.bottomMargin: 10
        anchors.leftMargin: 20
        anchors.topMargin: 20
    }

    TerminalTextArea{
        id: outputTextArea
        width: inputTextArea.width
        height: inputTextArea.height
        anchors.left: inputTextArea.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 20
        anchors.leftMargin: 20
        anchors.topMargin: 20
    }

    TextStreamField{
        id : textStreamField
        y: 375
        width: 164
        height: 27
        anchors.verticalCenter: clearOutputButton.verticalCenter
        anchors.left: outputTextArea.left
        anchors.leftMargin: 0
    }

    SendButton{
        id: sendButton
        y: 326
        width: 32
        height: 32
        anchors.verticalCenter: textStreamField.verticalCenter
        anchors.left: textStreamField.right
        anchors.leftMargin: 20

        onClicked:  {
            //send array of bytes or string
            sendToOutputRequest(outputConverters[outputSendModeComboBox.currentIndex](textStreamField.getText()))
        }
    }

    Common.MenuComboBox{
        id: outputSendModeComboBox
        y: 50
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 45
        anchors.rightMargin: 300
        prefixText: "Sending mode"
        elements: ["Send Ascii", "Send Uint8", "Send Uint16", "Send Uint32", "Send Uint64"]

        body.onActivated: outputDisplayModeChanged()
    }

    Common.MenuComboBox{
        id: inputDisplayComboBox
        anchors.verticalCenter: outputSendModeComboBox.verticalCenter
        anchors.right: outputSendModeComboBox.left
        anchors.rightMargin: 250

        prefixText: "Display as"
        elements: ["Ascii","Number","Hexadecimal","Binary"]

        body.onActivated: inputSendModeChanged()
    }

    Common.MenuTextButton{
        id: clearOutputButton
        x: 258
        y: 322
        height: 32
        anchors.right: outputTextArea.right
        anchors.bottom: outputSendModeComboBox.top
        anchors.bottomMargin: 10
        anchors.rightMargin: 0
        buttonText: "Clear output"

        onClicked: clearOutputRequested()
    }

    Common.MenuTextButton{
        id: freezeOutputButton
        x: 179
        y: 322
        anchors.verticalCenter: clearInputButton.verticalCenter
        anchors.right: clearInputButton.left
        anchors.rightMargin: 15
        buttonText: "Freeze input"

        onClicked: freezeInputRequested()
    }

    Common.MenuTextButton{
        id: clearInputButton
        y: 322
        anchors.verticalCenter: clearOutputButton.verticalCenter
        anchors.right: inputTextArea.right
        anchors.rightMargin: 0
        buttonText: "Clear input"

        onClicked: clearInputRequested()
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

    //IN NEXT VERSION
    //Common.MenuTextButton{
    //    id: chooseFileButton
    //    x: 544
    //    y: 393
    //    anchors.right: parent.right
    //    anchors.bottom: sendFileButton.bottom
    //    anchors.bottomMargin: 35
    //    anchors.rightMargin: 40
    //    buttonText: "Choose file..."
    //}
    //Common.MenuTextButton{
    //    id: sendFileButton
    //    y: 428
    //    anchors.left: fileTextField.left
    //    anchors.bottom: parent.bottom
    //    anchors.leftMargin: 0
    //    anchors.bottomMargin: 40
    //    buttonText: "Send file"
    //}
    //
    //TerminalTextArea{
    //    id : fileTextField
    //    x: 367
    //    y: 395
    //    width: 164
    //    height: 27
    //    anchors.verticalCenter: chooseFileButton.verticalCenter
    //    anchors.right: chooseFileButton.left
    //    anchors.rightMargin: 20
    //
    //}
}
