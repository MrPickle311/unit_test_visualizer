import QtQuick 2.15
import QtQuick.Controls 2.12
import "../common" as Common

Common.FrameRectangle {

    ScrollView {
        anchors.fill: parent
        clip: true

        Label {//replace with ListView in the future
                text: "ABC"
                font.pixelSize: 224
            }

    }
     z: 2
}
