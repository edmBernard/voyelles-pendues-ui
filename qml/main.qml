import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

Window {
    width: 480
    height: 640
    visible: true
    title: qsTr("Voyelles Pendues")
    id: mainWindow
    property string defaultColor: "#00ffffff"
    property string secondaryColor: "#aaaaaa"

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
        TabButton {
            contentItem: Text {
                text: qsTr("Best Score")
                font.bold: bar.currentIndex === 2
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

    Rectangle {
        id: titleContainer
        height: parent.height / 8
        color: mainWindow.defaultColor
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: bar.bottom
        anchors.topMargin: 0
        anchors.leftMargin: 0

        Text {
            id: titleLabel
            text: qsTr("Voyelles Pendues")
            anchors.fill: parent
            font.pixelSize: 24
            font.capitalization: "AllUppercase"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }


    StackLayout {
        id: layout
        anchors.top: titleContainer.bottom
        anchors.bottom:parent.bottom
        anchors.left: parent.left
        anchors.right:parent.right
        currentIndex: bar.currentIndex
        Rectangle {
            GamePage {
                width: parent.width
                height: parent.height
                gameBackend: gameBackend4x4
                defaultColor: mainWindow.defaultColor
                secondaryColor: mainWindow.secondaryColor
            }
        }
        Rectangle {
            GamePage {
                width: parent.width
                height: parent.height
                gameBackend: gameBackend5x5
                defaultColor: mainWindow.defaultColor
                secondaryColor: mainWindow.secondaryColor
            }
        }
        Rectangle {
            ScorePage {
                width: parent.width
                height: parent.height
                defaultColor: mainWindow.defaultColor
                secondaryColor: mainWindow.secondaryColor
            }
        }
    }
}
