import QtQuick 2.15
import QtQuick.Controls 2.12
import "../common" as Common

Rectangle {
    id: pageRectangle
    color: "white"
    clip: false

    property string portName: "none"

    signal openPortRequest()
    signal sendToOutputRequest(var data)

    signal outputDisplayModeChanged()
    signal inputSendModeChanged()

    height: 405
    width: 800

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

    function convertNmbrToBytes(str , bits){

        var data = parseInt(str)

        var byteArray = [];

        for ( let idx = 0; idx < bits; idx++ ){
            byteArray.push(0);
        }

        for ( let index = 0; index < byteArray.length; index ++ ) {
            var byte_ = data & 0xFF;
            byteArray [ index ] = byte_;
            data = (data - byte_) / 256 ;
        }

        return byteArray;
    }

    //accepts uint8tArray and removes all occurences of 'from' and replaces it with 'to' argument
    function replaceALl(data, from ,to){
        return data.toString().split(from).join(to)
    }

    function convertStrTo(str, base){
        return str.map(c => String.fromCharCode(c)).join(' ')
        //return str.split('').map(c => c.charCodeAt(0).toString(base)).join(' ')
    }

    function convertToChars(data){
        return String.fromCharCode(data)
    }

    property var inputConverters: [
        function(str) { return replaceALl(str,","," ") },
        function(str) { return convertStrTo(str,10) },
        function(str) { return convertStrTo(str,16) },
        function(str) { return convertStrTo(str,2) }
    ]

    property var outputConverters: [
        function(str) { return str;},
        function(str) { return convertNmbrToBytes(str,1) },//uint8
        function(str) { return convertNmbrToBytes(str,2) },//uint16
        function(str) { return convertNmbrToBytes(str,4) },//uint32
        function(str) { return convertNmbrToBytes(str,8) }//uint64
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

        //outputTextArea.appendText(outputConverters[outputSendModeComboBox.currentIndex](data))
    }

    function replaceOutputText(data){
        outputTextArea.setText(outputConverters[outputSendModeComboBox.currentIndex](data))
    }

    TextStreamField{
        id : textStreamField
        y: 375
        width: 164
        height: 27
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30
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

        onClicked:  {
            //send array of bytes or string
            sendToOutputRequest(outputConverters[outputSendModeComboBox.currentIndex](textStreamField.getText()))
        }
    }

    Common.MenuComboBox{
        id: outputSendModeComboBox
        y: 50
        anchors.verticalCenter: checkBoxLF.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 200
        anchors.verticalCenterOffset: -20
        prefixText: "Sending mode"
        elements: ["Send Ascii", "Send Uint8", "Send Uint16", "Send Uint32", "Send Uint64"]

        body.onActivated: outputDisplayModeChanged()
    }

    Common.MenuComboBox{
        id: inputDisplayComboBox
        anchors.verticalCenter: parent.verticalCenter

        prefixText: "Display as"
        elements: ["Ascii","Number","Hexadecimal","Binary"]

        body.onActivated: inputSendModeChanged()
    }

    CheckBox {
        id: checkBoxCR
        y: 368
        text: qsTr("Apend +CR")
        anchors.verticalCenter: textStreamField.verticalCenter
        anchors.left: textStreamField.right
        anchors.leftMargin: 60
    }

    CheckBox {
        id: checkBoxLF
        y: 401
        width: 102
        text: qsTr("Apend +LF")
        anchors.verticalCenter: checkBoxCR.verticalCenter
        anchors.left: checkBoxCR.right
        anchors.leftMargin: 30
    }

    Common.MenuTextButton{
        id: clearOutputButton
        x: 258
        y: 322
        height: 32
        anchors.right: outputTextArea.right
        anchors.bottom: checkBoxCR.top
        anchors.bottomMargin: 15
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
