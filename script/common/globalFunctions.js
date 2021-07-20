.import QtQuick 2.15 as QtQuick

function createComponent(directory , parent)
{
    var comp = Qt.createComponent(directory)

    if( comp.status !== QtQuick.Component.Ready)
    {
        if( comp.status === QtQuick.Component.Error )
            console.debug("Error:" + comp.errorString() );
        return;
    }

    return comp.createObject(parent)
}

function createWindow(directory)
{
    //parent of each window is mainWindow.contentItem
    return createComponent(directory , mainWindow.contentItem );
}
