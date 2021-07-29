.import "../common/globalFunctions.js" as Global

function openPort(){
    var port_names = Object.keys(pagesId)
    bridge.openPort( port_names[bar.currentIndex])
}

function createTerminalWindow()
{
    return Global.createWindow("qrc:/script/terminal/TerminalWindow.qml")
}

function addPortPage(port_name){
    pagesId[port_name] = repeater.count
    outputDataList.push("")
    inputDataList.push("")
    isInputFreezed.push(false)
    repeater.model = Object.keys(pagesId) //repeater does not refresh itself automatically
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
    repeater.model = port_names//refresh model
}

function changePortPage(){
    terminalPage.replaceInputText(inputDataList[bar.currentIndex])
    terminalPage.replaceOutputText(outputDataList[bar.currentIndex])
}

//update output text view
function refreshOutput(data){
    var view_data = typeof(data) == "string" ?  data : data.map(String).toString() + '  '
    outputDataList[bar.currentIndex] += view_data
    terminalPage.appendTextToOutput(view_data)
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


function concatUint8Arrays(a, b) { // a, b TypedArray of same type
    var c = new Uint8Array(a.length + b.length);
    c.set(a, 0);
    c.set(b, a.length);
    return c;
}

//it only updates view
function receiceData(port_name , data){
    if(!isInputFreezed[pagesId[port_name]])//if port is not freezed
    {
        var uint8 = new Uint8Array(data);
        inputDataList[pagesId[port_name]] = concatUint8Arrays(inputDataList[pagesId[port_name]] ,  uint8)
        refreshInput()
    }
}

function sendData(data){
    var port_name = getKeyByValue(pagesId , bar.currentIndex )
    bridge.sendData( port_name , typeof(data) == "string" ?  data : String.fromCharCode(...data) )
}
