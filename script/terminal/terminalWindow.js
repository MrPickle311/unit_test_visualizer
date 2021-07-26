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
        inputDataTextList.push("")
    }
    repeater.model = port_names
}

function refreshOutput(){
    terminalPage.replaceOutputText(outputDataTextList[bar.currentIndex])
}

function refreshInput(){
    terminalPage.replaceInputText(inputDataTextList[bar.currentIndex])
}

function receiceData(port_name , data){
    inputDataTextList[pagesId[port_name]] += data
    refreshInput()
}

function sendData(data){
    var port_name = getKeyByValue(pagesId , bar.currentIndex )
    bridge.sendData( port_name , data )
    outputDataTextList[bar.currentIndex] += data
    refreshOutput()
}
