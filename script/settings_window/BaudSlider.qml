import QtQuick 2.15
import QtQuick.Controls 2.15
import "baudSliderLogic.js" as BaudSliderLogic

Item{
    id: baudSlider

    Slider{
        id: slider
        from: 0
        to: 7
        stepSize: 1
        snapMode: Slider.SnapAlways
        onPositionChanged:{
            baudRateText.text = BaudSliderLogic.getBaudRateText(slider.value)
            baudSlider.positionChanged(baudRateText.text)
        }
    }

    Text {
        id: baudRateText
        anchors.left: slider.right
        anchors.leftMargin: 5
        text: "1200"
        anchors.verticalCenter: slider.verticalCenter
    }

    Text {
        id: prefix
        text: "Baud rate"
        anchors.verticalCenter: slider.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: -1*prefix.paintedWidth - 5
    }

    property alias text: baudRateText.text

    signal positionChanged(string value)

}
