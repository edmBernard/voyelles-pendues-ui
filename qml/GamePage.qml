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
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            onPressed: gameBackend.generateNewPuzzle()

            contentItem: Text {
                text: qsTr("New")
                color: "#aaaaaa"
                font.pixelSize: 24
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                implicitWidth: 100
                implicitHeight: 40
                border.width: 0
                color: "#ffffff"
            }
        }
        Rectangle {
            id: wordCounterContainer
            anchors.horizontalCenter: headerContainer.horizontalCenter
            anchors.verticalCenter: headerContainer.verticalCenter
            Text {
                id: wordSeparatorLabel
                text: "/"
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.verticalCenter: parent.verticalCenter;
                font.pixelSize: 24
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            Text {
                id: wordFoundLabel
                text: gameBackend.getFound()
                anchors.verticalCenter: parent.verticalCenter;
                anchors.right: wordSeparatorLabel.left
                font.pixelSize: 24
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            Text {
                id: wordTotalLabel
                text: gameBackend.getTotal()
                anchors.verticalCenter: parent.verticalCenter;
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
                id: scoreValue
                text: "0"
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.rightMargin: 10
                font.pixelSize: 24
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            Text {
                id: scoreLabel
                text: "Score: "
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.right: scoreValue.left
                font.pixelSize: 12
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
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            onClicked: gameBackend.previousWord()

            contentItem: Text {
                text: "<"
                color: "#aaaaaa"
                font.pixelSize: 36
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                implicitWidth: 100
                implicitHeight: 40
                border.width: 0
                color: "#ffffff"
            }
        }
        Button {
            id: wildCardButtonNext
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            onClicked: gameBackend.nextWord()

            contentItem: Text {
                text: ">"
                color: "#aaaaaa"
                font.pixelSize: 36
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                implicitWidth: 100
                implicitHeight: 40
                border.width: 0
                color: "#ffffff"
            }
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
                    width: 30
                    Text {
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text: empty == "0" ? letter : "_"
                        font.pixelSize: 36
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
            cellWidth: 60
            cellHeight: 60
            width: 60 * gameBackend.getGridSize()
            height: 60 * gameBackend.getGridSize()
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            focus: true
            interactive: false
            model: gameBackend.getGrid()

            delegate: Item {
                Text {
                    anchors.fill: parent
                    id: gridLabel
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    text: letter
                    font.capitalization: Font.AllUppercase
                    font.pixelSize: 48
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
            text: "Erwan Bernard"
            anchors.leftMargin: 10
            anchors.fill: parent
            font.pixelSize: 12
            color: "#aaaaaa"
            verticalAlignment: Text.AlignVCenter
            minimumPointSize: 8
            minimumPixelSize: 8
        }
    }
}
