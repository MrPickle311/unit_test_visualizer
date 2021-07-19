.import QtQuick 2.15 as QtQuick

function createWindow(directory)
{
    var window = Qt.createComponent(directory)
    if( window.status !== QtQuick.Component.Ready )
    {
        if( window.status === QtQuick.Component.Error )
            console.debug("Error:"+ window.errorString() );
        return;
    }
    window.createObject(mainWindow.contentItem)//parent of each window is mainWindow
    return window
}
