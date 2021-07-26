import QtQuick 2.15
import QtQuick.Controls 2.12
import "../common" as Common

import Qt.singletons.bridge 1.0

import "testWindowLogic.js" as Logic

Common.FrameRectangle {

    Component.onCompleted: {
        TestsBridge.sendTestCase.connect(Logic.appendTestCase)
        TestsBridge.sendUnitTest.connect(Logic.appendUnitTest)
    }

    ListModel {
        id: testCasesModel
    }

    property alias model: testCasesModel

    Component {
        id: testCaseDelegate

        Column {
            id: col
            width: 200

            Rectangle {
                id: testCaseItem
                border.color: "black"
                border.width: 2
                color: case_color
                height: 30
                width: 350

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    x: 15
                    font.pixelSize: 16
                    text: testCaseName
                }

                MouseArea {
                    anchors.fill: parent

                    // Toggle the 'collapsed' property
                    onClicked: testCasesModel.setProperty(index, "collapsed", !collapsed)
                    z: 3
                }
            }

            Loader {
                id: testCaseLoader
                visible: !collapsed
                property variant subItemModel : unitTests
                sourceComponent: collapsed ? undefined : unitTestDelegate//'To unload the currently loaded object, set this property to undefined.'
                onStatusChanged: if (status == Loader.Ready) item.model = subItemModel
            }
        }
    }

    Component {
        id: unitTestDelegate
        Column {
            id: column
            property alias model : unitTestRepeater.model
            width: 200

            Repeater {
                id: unitTestRepeater
                onItemAdded: console.log("xd")


                delegate: Rectangle {
                    x : 20
                    anchors.margins: 10
                    height: 80
                    width: 430
                    border.color: "black"
                    border.width: 1

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        x: 30
                        font.pixelSize: 12
                        text: "Expression name: " +  expressionName + "\n" +
                              "Type: " + typeDescriptor + "\n" +
                              "Current value : " + currentValue + "\n" +
                              "Expected value : " + expectedValue + "\n" +
                              "Test result: " + testResult
                    }

                    Component.onCompleted : {
                        if(testResult == "Failed")
                            color = "lightcoral"
                        else color = "lightgreen"
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
                delegate: testCaseDelegate
            }
    }

    z: 2
}
