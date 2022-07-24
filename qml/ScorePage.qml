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
        anchors.fill: parent

        orientation: ListView.VerticalTopToBottom
        interactive: false

        model: scoreBackend.getBestScores()

        delegate: Rectangle {
            width: parent.width
            height: indexContainer.height
            anchors.left: parent.left

            Text {
                id: indexContainer
                width: parent.width / 10
                anchors.left: parent.left
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                text: index
                color: score.defaultColor
            }
            Text {
                id: dateContainer
                anchors.right: parent.right
                width: parent.width / 3
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter

                text: date * 3000
                color: score.defaultColor
            }
            Text {
                anchors.left: indexContainer.right
                anchors.right: dateContainer.left
                text: score * 10
                color: score.secondaryColor
            }
        }
    }
}
