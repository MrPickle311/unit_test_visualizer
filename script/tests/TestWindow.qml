import QtQuick 2.15
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "../common" as Common

import Qt.singletons.bridge 1.0

Common.FramelessWindow{
    id: testWindow

    height: 400
    width: 400

    TestResultsArea{
        id: testResultsArea
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: testWindow.closeButton.bottom
        anchors.bottom: parent.bottom

        anchors.rightMargin: 20
        anchors.leftMargin: 20
        anchors.bottomMargin: 20
        anchors.topMargin: 5
    }

    Common.RefreshButton{
        id: refreshButton
        y: 36
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.top: parent.top
        anchors.topMargin: 5
        z:2

        onClicked: {
            testResultsArea.model.clear()
            TestsBridge.run()
        }
    }

    Common.SaveButton{
        id: saveButton
        anchors.verticalCenter: refreshButton.verticalCenter
        anchors.left: refreshButton.right
        anchors.leftMargin: 10
        z: 2

    }

}
