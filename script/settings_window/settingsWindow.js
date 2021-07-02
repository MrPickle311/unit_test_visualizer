function createSettignsWindow()
{
    var settings_window = Qt.createComponent("qrc:/script/settings_window/SettingsWindow.qml")
    settings_window.createObject(mainWindow)
}

class SettingsWindow
{

}
