import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick 2.15
import "buttonLogic.js" as ButtonLogic

Item {

      id: menuButton
      property int size: 128
      width: size
      height: size
      property bool isRotationEnabled: false
      property color color: "white"
      property color hoverColor: "lightsteelblue"
      property color pressColor: "slategray"
      property int borderWidth: 3
      property string iconDir: ""
      scale:  ButtonLogic.fitScaleToState(menuButton)
      onEnabledChanged: state = ""

      signal clicked

      Behavior on scale {
          NumberAnimation {
              duration: 100
              easing.type: Easing.OutQuart
          }
      }



      Rectangle {
          id: rectangleButton
          anchors.fill: parent
          color: menuButton.enabled ? menuButton.color : "gray"
          border.color: "white"
          border.width: borderWidth

          Image{
              id : image
              anchors.fill: parent
              source: iconDir
              fillMode: Image.PreserveAspectFit
          }

          Behavior on rotation {
              NumberAnimation {
                  duration: 50
                  easing.type: Easing.Linear
              }
          }
      }

      states: [
          State {
              name: "Hovering"
              PropertyChanges {
                  target: rectangleButton
                  color: hoverColor
                  border.color: "black"
              }
          },
          State {
              name: "Pressed"
              PropertyChanges {
                  target: rectangleButton
                  color: pressColor
                  border.color: "black"
              }
          },
          State {
              name: "Clicked"
              PropertyChanges {
                  target: rectangleButton
                  rotation: isRotationEnabled ? 90 : 0
              }
          }
      ]

      transitions: [
          Transition {
              from: ""; to: "Hovering"
              ColorAnimation { duration: 300 ; easing.type : Easing.Linear }
          }
      ]

      MouseArea {
          hoverEnabled: true//to grab every mouse event
          anchors.fill: menuButton
          onEntered: { menuButton.state = "Hovering"}
          onExited:  {
                menuButton.state = ""
                rectangleButton.rotation = 0
          }
          onClicked: {
              rectangleButton.rotation =  isRotationEnabled ? 90 : 0
              menuButton.clicked()
          }
          onPressed: { menuButton.state = "Pressed" }
          onReleased: ButtonLogic.switchState(this , menuButton)
      }
}
