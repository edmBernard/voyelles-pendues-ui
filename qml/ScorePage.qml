import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    id: score
    property string defaultColor
    property string secondaryColor
    property string notificationColor: "#ffffff"
    property string primaryColor: "#000000"
    property string hintColor: "#5555aa"

    ListView {
        anchors.bottom: parent.bottom
        height: parent.height * 2 / 3
        anchors.left: parent.left
        anchors.right: parent.right

        verticalLayoutDirection: ListView.TopToBottom
        interactive: false

        model: scoreBackend.getBestScores()
        delegate: Rectangle {
          Text {
            text: index
          }
          Text {
            text: score
          }
          Text {
            text: date
          }
        }
    }

    Rectangle  {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0; color: "#ffffffff" }
            GradientStop { position: 0.5; color: "#ffffffff" }
            GradientStop { position: 1; color: "#33ffffff" }
        }
    }
}
