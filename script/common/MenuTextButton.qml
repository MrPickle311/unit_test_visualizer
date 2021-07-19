import QtQuick 2.12

MenuButton {
    id: menuButton
    usesImage: false
    property alias buttonText: buttonTxt.text
    height: 32
    width: buttonTxt.paintedWidth + 5
    borderWidth: 1
    z: 2
    hoverColor: "skyblue"
    Text {
        id: buttonTxt
        text: qsTr("Text")
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

    }
}
