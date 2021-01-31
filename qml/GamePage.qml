import QtQuick 2.12
import QtQuick.Controls 2.12

Item {

    Connections {
        target: gameBackend
        function onUpdateScore(score) {
            scoreLabel.text = score;
        }
        function onUpdateWord() {
            wildCardLabel.text = gameBackend.getWord();
            wordIndexLabel.text = gameBackend.getIndex() + 1;
            wordRemainingLabel.text = gameBackend.getTotalRemaining();
        }
        function onUpdateGrid() {
            grid.model = gameBackend.getGrid();
        }
    }

    Rectangle {
        id: titleContainer
        height: parent.height / 8
        color: "#ffffff"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.leftMargin: 0

        Text {
            id: titleLabel
            text: qsTr("Voyelles Pendues")
            anchors.fill: parent
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    Rectangle {
        id: headerContainer
        height: parent.height / 16
        color: "#ffff00"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: titleContainer.bottom
        anchors.topMargin: 0
        anchors.leftMargin: 0

        Button {
            id: newPuzzleButton
            text: qsTr("New")
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            onPressed: gameBackend.generateNewPuzzle()
        }
        Rectangle {
            id: wordCounterContainer
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: newPuzzleButton.right
            anchors.right: scoreLabel.left
            Text {
                id: wordIndexLabel
                text: gameBackend.getIndex()
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                font.pixelSize: 12
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            Text {
                id: wordRemainingLabel
                text: gameBackend.getTotalRemaining()
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                font.pixelSize: 12
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
        Text {
            id: scoreLabel
            text: "0"
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

    }

    Rectangle {
        id: wildCard
        height: parent.height / 8
        color: "#ffffff"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: headerContainer.bottom
        anchors.leftMargin: 0

        Button {
            id: wildCardButtonPrevious
            text: qsTr("<")
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            onClicked: gameBackend.previousWord()
        }
        Button {
            id: wildCardButtonNext
            text: qsTr(">")
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            onClicked: gameBackend.nextWord()
        }
        Text {
            id: wildCardLabel
            text: gameBackend.getWord()
            anchors.fill: parent
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    Rectangle {
        id: gridContainer
        height: parent.height / 4
        color: "#ff00ff"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: wildCard.bottom
        anchors.bottom: footerContainer.top
        anchors.leftMargin: 0

        GridView {
            id: grid
            anchors.fill: parent
            cellWidth: 100; cellHeight: 100
            focus: true
            model: gameBackend.getGrid()

            highlight: Rectangle { width: 80; height: 80; color: "lightsteelblue" }

            delegate: Item {
                width: 100; height: 100

                Text {
                    anchors { horizontalCenter: parent.horizontalCenter }
                    text: modelData
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: parent.GridView.view.currentIndex = index
                }
            }
        }
    }

    Rectangle {
        id: footerContainer
        height: parent.height / 16
        color: "#00ffff"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.leftMargin: 0

        Text {
            id: footerLabel
            text: qsTr("Erwan Bernard")
            anchors.fill: parent
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            minimumPointSize: 8
            minimumPixelSize: 8
        }
    }
}
