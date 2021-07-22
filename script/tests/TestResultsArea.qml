import QtQuick 2.15
import QtQuick.Controls 2.12
import "../common" as Common

Common.FrameRectangle {

    Component.onCompleted: {
        testCasesModel.append({"testCaseName" : "fruits" , "collapsed" : true ,"unitTests" : [ { "itemName": "Orange"} ]  })
        testCasesModel.get(0).categoryName = "sds"
        testCasesModel.get(0).unitTests = [{ "expressionName": "Orange"}]
        testCasesModel.get(0).unitTests.append({ "expressionName": "Apple"})
        testCasesModel.get(0).unitTests.append({ "expressionName": "Strawberry"})
        testCasesModel.append({"testCaseName" : "fruits" , "collapsed" : true ,"unitTests" : []})
        appendTestCase("sd")
        appendTestCase("sd")
        appendTestCase("sd")
        appendTestCase("sd")
        appendTestCase("sd")
        testCasesModel.get(1).unitTests.append({ "expressionName": "Strawberry"})
    }

    function appendTestCase(test_case_name){
        testCasesModel.append({"testCaseName" : test_case_name , "collapsed" : true ,"unitTests" : []  })
    }

    function appendUnitTest(test_case_nmbr , unit_test , is_range_test){
        testCasesModel.get(test_case_nmbr).unitTests.append({"expressionName": unit_test.name ,
                                                             "typeDescriptor": unit_test.typeDescriptor ,
                                                             "currentValue"  : unit_test.currentValue ,

                                                             "expectedValue" : is_range_test ?
                                                                                   "A value between " + unit_test.lowerValue +
                                                                                   " and " + unit_test.upperValue
                                                                                 :
                                                                                   unit_test.expectetedValue ,

                                                             "testResult"    : unit_test.testResult} );
    }

    ListModel {
        id: testCasesModel
    }

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
