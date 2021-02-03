import QtQuick 2.12
import QtQuick.Controls 2.12

Item {

    Connections {
        target: gameBackend
        function onUpdateScore(score) {
            scoreValue.text = score;
        }
        function onUpdateMeta() {
            wordFoundLabel.text = gameBackend.getFound();
            wordTotalLabel.text = gameBackend.getTotal();
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
            font.capitalization: "AllUppercase"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    Rectangle {
        id: headerContainer
        height: parent.height / 16
        color: "#ffffff"
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
                id: wordFoundLabel
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
                anchors.left: wordFoundLabel.right
                font.pixelSize: 24
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            Text {
                id: wordTotalLabel
                text: gameBackend.getTotal()
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: wordSeparatorLabel.right
                font.pixelSize: 24
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
        Rectangle {
            id: scoreContainer
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right

            Text {
                id: scoreLabel
                text: "Score: "
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                font.pixelSize: 24
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            Text {
                id: scoreValue
                text: "0"
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                font.pixelSize: 24
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
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
        Rectangle {
            id: wildCardContainer
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: wildCardButtonPrevious.right
            anchors.right: wildCardButtonNext.left

            ListView {
                id: wildCardView
                width: contentWidth
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                orientation: ListView.Horizontal
                interactive: false

                model: gameBackend.getWord()

                delegate: Item {
                    width: 20
                    Text {
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text: empty == "0" ? letter : "_"
                        font.pixelSize: 24
                        font.bold: empty == "1"
                    }
                }
            }
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
            width: 60 * gameBackend.getGridSize()
            height: 60 * gameBackend.getGridSize()
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            cellWidth: 60; cellHeight: 60
            focus: true
            interactive: false
            model: gameBackend.getGrid()

            delegate: Item {
                width: 60; height: 60
                Text {
                    id: gridLabel
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    text: letter
                    font.capitalization: Font.AllUppercase
                    font.pixelSize: 24
                    color: bloom == "0" ? "#aaaaaa" : "#000000"
                    font.bold: selected == "1"


                }
            }
            MultiPointTouchArea {
                anchors.fill: parent
                mouseEnabled: true
                maximumTouchPoints: 1
                onUpdated: function(touchPoints) {
                    let selectedIndex= grid.indexAt(touchPoints[0].x, touchPoints[0].y);
                    gameBackend.addLetter(selectedIndex);
                }
                onReleased: {
                    gameBackend.cleanLetter();
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
