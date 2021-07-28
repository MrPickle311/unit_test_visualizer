.import "../common/globalFunctions.js" as Global

function createTerminalWindow()
{
    return Global.createWindow("qrc:/script/terminal/TerminalWindow.qml")
}

function addPortPage(port_name){
    pagesId[port_name] = repeater.count
    outputDataList.push("")
    inputDataList.push("")
    isInputFreezed.push(false)
    var array = Object.keys(pagesId) //repeater does not refresh itself automatically
    repeater.model = array
}

function getKeyByValue(object, value) {
  return Object.keys(object).find(key => object[key] === value);
}

function tryRestore(){
    var port_names = bridge.restorePorts()
    for(var i = 0 ; i < port_names.length ; i++){
        pagesId[ port_names[i] ] = i
        outputDataList.push("")
        isInputFreezed.push(false)
        inputDataList.push(new Uint8Array(data))
    }
    repeater.model = port_names
}

function changePortPage(){
    terminalPage.replaceInputText(inputDataList[currentIndex])
    terminalPage.replaceOutputText(outputDataList[currentIndex])
}

function refreshOutput(data){
    terminalPage.appendTextToOutput(data)
}

function resetOutput(){
    terminalPage.resetOutputTextArea()
    outputDataList[bar.currentIndex] = []
}

function refreshInput(){
    terminalPage.replaceInputText(inputDataList[bar.currentIndex])
}

function resetInput(){
    terminalPage.resetInputTextArea()
    inputDataList[bar.currentIndex] = []
}


function concatTypedArrays(a, b) { // a, b TypedArray of same type
    var c = new Uint8Array(a.length + b.length);
    c.set(a, 0);
    c.set(b, a.length);
    return c;
}

function receiceData(port_name , data){
    if(!isInputFreezed[pagesId[port_name]])//if port is not freezed
    {
        var uint8 = new Uint8Array(data);
        inputDataList[pagesId[port_name]] = concatTypedArrays(inputDataList[pagesId[port_name]] ,  uint8)
        refreshInput()
    }
}

function sendData(data){
    var port_name = getKeyByValue(pagesId , bar.currentIndex )
    bridge.sendData( port_name , data )
}
