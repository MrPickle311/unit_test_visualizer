function createWindow(directory)
{
    var settings_window = Qt.createComponent(directory)
    settings_window.createObject(mainWindow)
}
