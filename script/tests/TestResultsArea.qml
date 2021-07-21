import QtQuick 2.15
import QtQuick.Controls 2.12
import "../common" as Common

Common.FrameRectangle {

    Component.onCompleted: {
        testCasesModel.append({"categoryName" : "fruits" , "collapsed" : true ,"subItems" : [ { "itemName": "Orange"} ]  })
    }

    ScrollView {
        anchors.fill: parent
        clip: true

        ListView {
                anchors.fill: parent
                model: testCasesModel
                delegate: categoryDelegate
            }

            ListModel {
                id: testCasesModel

                    ListElement {
                        categoryName: ""
                        collapsed: true

                        // A ListElement can't contain child elements, but it can contain
                        // a list of elements. A list of ListElements can be used as a model
                        // just like any other model type.
                        subItems: []
                    }
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
                            text: categoryName
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
                        property variant subItemModel : subItems
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
                            width: 150
                            border.color: "black"
                            border.width: 2

                            Text {
                                anchors.verticalCenter: parent.verticalCenter
                                x: 30
                                font.pixelSize: 18
                                text: itemName
                            }
                        }
                    }
                }
            }
    }
     z: 2
}
