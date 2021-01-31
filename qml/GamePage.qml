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
            font.pixelSize: 24
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
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            Text {
                id: wordIndexLabel
                text: gameBackend.getIndex()
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                font.pixelSize: 24
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            Text {
                id: wordSeparatorLabel
                text: "/"
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: wordIndexLabel.right
                font.pixelSize: 24
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            Text {
                id: wordRemainingLabel
                text: gameBackend.getTotalRemaining()
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: wordSeparatorLabel.right
                font.pixelSize: 24
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
            font.pixelSize: 24
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
            font.pixelSize: 24
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    Rectangle {
        id: gridContainer
        color: "#ffffff"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: wildCard.bottom
        anchors.bottom: footerContainer.top

        GridView {
            id: grid
            width: 300
            height: 300
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            cellWidth: 60; cellHeight: 60
            focus: true
            model: gameBackend.getGrid()

            delegate: Item {
                width: 60; height: 60

                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    text: modelData
                    font.capitalization: Font.AllUppercase
                    font.pixelSize: 24
                    font.bold: true
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
        height: 16
        color: "#ffffff"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.leftMargin: 0

        Text {
            id: footerLabel
            text: qsTr("Erwan Bernard")
            anchors.fill: parent
            font.pixelSize: 12
            verticalAlignment: Text.AlignVCenter
            minimumPointSize: 8
            minimumPixelSize: 8
        }
    }
}
