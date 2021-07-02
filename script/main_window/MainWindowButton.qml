import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick 2.15

//Implementation of the Button control.
  Item {
      id: button
      width: 128
      height: 128
      property color color: "white"
      property color hoverColor: "#aaaaaa"
      property color pressColor: "slategray"
      //property int borderWidth: 1
      //property int borderRadius: 2
      scale: state === "Pressed" ? 0.96 : 1.0
      onEnabledChanged: state = ""
      signal clicked

      //define a scale animation
      Behavior on scale {
          NumberAnimation {
              duration: 100
              easing.type: Easing.InOutQuad
          }
      }

      //Rectangle to draw the button
      Rectangle {
          id: rectangleButton
          anchors.fill: parent
          //radius: borderRadius
          color: button.enabled ? button.color : "grey"
          //border.width: borderWidth
         // border.color: "black"

          Image{
              id : image
              anchors.fill: parent
              source: "qrc:/data/main_window/settings2.png"
              fillMode: Image.PreserveAspectFit
          }

      }

      //change the color of the button in differen button states
      states: [
          State {
              name: "Hovering"
              PropertyChanges {
                  target: rectangleButton
                  color: hoverColor
              }
          },
          State {
              name: "Pressed"
              PropertyChanges {
                  target: rectangleButton
                  color: pressColor
              }
          }
      ]

      //define transmission for the states
      transitions: [
          Transition {
              from: ""; to: "Hovering"
              ColorAnimation { duration: 200 }
          },
          Transition {
              from: "*"; to: "Pressed"
              ColorAnimation { duration: 10 }
          }
      ]

      //Mouse area to react on click events
      MouseArea {
          hoverEnabled: true
          anchors.fill: button
          onEntered: { button.state='Hovering'}
          onExited: { button.state=''}
          onClicked: { button.clicked();}
          onPressed: { button.state="Pressed" }

          onReleased: {
              if (containsMouse)
                button.state="Hovering";
              else
                button.state="";
          }
      }
  }
