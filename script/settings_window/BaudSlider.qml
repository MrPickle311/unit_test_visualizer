import QtQuick 2.15
import QtQuick.Controls 2.15
import "baudSliderLogic.js" as BaudSliderLogic

Item{
    id: item1

    Slider{
        id: slider
        from: 0
        to: 7
        stepSize: 1
        snapMode: Slider.SnapAlways
        onPositionChanged: baudRateText.text = BaudSliderLogic.getBaudRateText(slider.value)
    }

    Text {
        id: baudRateText
        anchors.left: slider.right
        anchors.leftMargin: 5
        text: "300"
        anchors.verticalCenter: slider.verticalCenter
    }

}
