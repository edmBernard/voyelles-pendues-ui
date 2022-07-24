import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    id: root
    property string defaultColor
    property string secondaryColor
    property string notificationColor: "#ffffff"
    property string primaryColor: "#000000"
    property string hintColor: "#5555aa"

    Rectangle {
        id: buttonBar
        height: 40
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        color: root.defaultColor

        Button {
            id: resetScoreButton
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            onPressed: scoreBackend.resetBestScores()

            contentItem: Text {
                id: resetText
                text: qsTr("Reset")
                color: root.secondaryColor
                font.pixelSize: 24
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                anchors.fill: parent
                border.width: 0
                color: root.defaultColor
            }
        }
    }

    ListView {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: buttonBar.top
        anchors.top: parent.top

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
                color: root.secondaryColor
            }
            Text {
                id: dateContainer
                anchors.right: parent.right
                width: parent.width / 3
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter

                text: date
                color: root.secondaryColor
            }
            Text {
                anchors.left: indexContainer.right
                anchors.right: dateContainer.left
                text: score
                color: root.primaryColor
            }
        }
    }
}
