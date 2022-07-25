import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

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
            onPressed: function() {
                scoreBackend4x4.resetBestScores();
                scoreBackend5x5.resetBestScores();
            }

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

    TabBar {
        id: bar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        TabButton {
            contentItem: Text {
                text: qsTr("4x4")
                color: mainWindow.secondaryColor
                font.bold: bar.currentIndex === 0
                font.pixelSize: 18
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                border.width: 0
                color: mainWindow.defaultColor
            }

        }
        TabButton {
            contentItem: Text {
                text: qsTr("5x5")
                font.bold: bar.currentIndex === 1
                color: mainWindow.secondaryColor
                font.pixelSize: 18
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                border.width: 0
                color: mainWindow.defaultColor
            }
        }
    }

    StackLayout {
        id: layout
        anchors.top: bar.bottom
        anchors.bottom: buttonBar.top
        anchors.left: parent.left
        anchors.right: parent.right
        currentIndex: bar.currentIndex

        Rectangle {
            ScoreList {
                width: parent.width
                height: parent.height
                defaultColor: root.defaultColor
                secondaryColor: root.secondaryColor
                scoreBackend: scoreBackend4x4
            }
        }
        Rectangle {
            ScoreList {
                width: parent.width
                height: parent.height
                defaultColor: root.defaultColor
                secondaryColor: root.secondaryColor
                scoreBackend: scoreBackend5x5
            }
        }
    }


}
