.import "../common/globalFunctions.js" as Global

function createTerminalWindow()
{
    return Global.createWindow("qrc:/script/terminal/TerminalWindow.qml")
}

function addPortPage(port_name){
    pagesId[port_name] = repeater.count
    outputDataTextList.push("")
    inputDataTextList.push("")
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
        outputDataTextList.push("")
        inputDataTextList.push(new Uint8Array(data))
    }
    repeater.model = port_names
}

function refreshOutput(data){
    //outputDataTextList[bar.currentIndex]
    terminalPage.appendTextToOutput(data)
    //terminalPage.replaceOutputText(outputDataTextList[bar.currentIndex])
}

function refreshInput(){
    terminalPage.replaceInputText(inputDataTextList[bar.currentIndex])
}

function concatTypedArrays(a, b) { // a, b TypedArray of same type
    var c = new Uint8Array(a.length + b.length);
    c.set(a, 0);
    c.set(b, a.length);
    return c;
}

function receiceData(port_name , data){
    var uint8 = new Uint8Array(data);
    //console.log("uint8 start " + "   "  + uint8 + "  uint8 END")
    inputDataTextList[pagesId[port_name]] = concatTypedArrays(inputDataTextList[pagesId[port_name]] ,  uint8)
    //console.log("array   " + inputDataTextList[pagesId[port_name]] + "   array END ")
    refreshInput()
}

function sendData(data){
    var port_name = getKeyByValue(pagesId , bar.currentIndex )
    bridge.sendData( port_name , data )
}
