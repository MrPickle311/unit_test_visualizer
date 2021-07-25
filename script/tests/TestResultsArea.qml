import QtQuick 2.15
import QtQuick.Controls 2.12
import "../common" as Common

import Qt.singletons.bridge 1.0

import "testWindowLogic.js" as Logic

Common.FrameRectangle {

    Component.onCompleted: {
        logic = new Logic.TestsLogic(testCasesModel)
        TestsBridge.sendTestCase.connect(logic.appendTestCase)
        TestsBridge.sendUnitTest.connect(logic.appendUnitTest)

    }

    property var logic: undefined

    ListModel {
        id: testCasesModel
    }

    property alias model: testCasesModel

    Component {
        id: categoryDelegate

        Column {
            property alias rc: categoryItem.right
            id: col
            width: 200

            Rectangle {
                id: categoryItem
                border.color: "black"
                border.width: 5
                color: "white"
                height: 50
                width: 200

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    x: 15
                    font.pixelSize: 24
                    text: testCaseName
                }

                Rectangle {
                    color: "red"
                    width: 30
                    height: 30
                    anchors.right: parent.right
                    anchors.rightMargin: 15
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent

                        // Toggle the 'collapsed' property
                        onClicked: testCasesModel.setProperty(index, "collapsed", !collapsed)
                        z:3
                    }
                }
            }

            Loader {
                id: subItemLoader
                visible: !collapsed
                property variant subItemModel : unitTests
                sourceComponent: collapsed ? undefined : subItemColumnDelegate//'To unload the currently loaded object, set this property to undefined.'
                onStatusChanged: if (status == Loader.Ready) item.model = subItemModel
            }
        }

    }

    Component {
        id: subItemColumnDelegate
        Column {
            property alias model : subItemRepeater.model
            width: 200
            Repeater {
                id: subItemRepeater
                delegate: Rectangle {
                    x : 20
                    anchors.margins: 10
                    color: "#cccccc"
                    height: 40
                    width: 320
                    border.color: "black"
                    border.width: 2

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        x: 30
                        font.pixelSize: 12
                        text: expressionName + " " + typeDescriptor + " " + currentValue + " " + expectedValue + " " + testResult
                    }
                }
            }
        }
    }

    ScrollView {
        anchors.fill: parent
        clip: true

        ListView {
                anchors.fill: parent
                model: testCasesModel
                delegate: categoryDelegate
            }
    }

    z: 2
}
