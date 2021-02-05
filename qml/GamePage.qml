import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    id: root
    property string notificationColor: root.defaultColor
    property string defaultColor: "#00ffffff"
    property string primaryColor: "#000000"
    property string secondaryColor: "#aaaaaa"
    property string hintColor: "#5555aa"

    Connections {
        target: gameBackend
        function onUpdateScore(score) {
            scoreValue.text = score;
        }
        function onUpdateMeta() {
            wordFoundLabel.text = gameBackend.getFound();
            wordTotalLabel.text = gameBackend.getTotal();
        }
        function onNotify(message, color) {
            notificationLabel.text = message;
            root.notificationColor = color;
            allAnimation.start();
        }
    }

    Rectangle {
        id: titleContainer
        height: parent.height / 8
        color: root.defaultColor
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
        id: buttonBar
        height: 40
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: titleContainer.bottom
        color: root.defaultColor

        Button {
            id: newPuzzleButton
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            onPressed: gameBackend.generateNewPuzzle()

            contentItem: Text {
                text: qsTr("New")
                color: root.secondaryColor
                font.pixelSize: 24
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                implicitWidth: 60
                implicitHeight: 40
                border.width: 0
                color: root.defaultColor
            }
        }
        Button {
            id: hintButton
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: newPuzzleButton.right
            anchors.leftMargin: 10
            onPressed: gameBackend.getHint()

            contentItem: Text {
                text: qsTr("Hint")
                color: root.secondaryColor
                font.pixelSize: 24
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                implicitWidth: 80
                implicitHeight: 40
                border.width: 0
                color: root.defaultColor
            }
        }
    }

    Rectangle {
        id: headerContainer
        height: parent.height / 16
        color: root.defaultColor
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: buttonBar.bottom
        anchors.topMargin: 0
        anchors.leftMargin: 0


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
                color: root.secondaryColor
                font.pixelSize: 36
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                implicitWidth: 60
                implicitHeight: 40
                border.width: 0
                color: root.defaultColor
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
                color: root.secondaryColor
                font.pixelSize: 36
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                implicitWidth: 60
                implicitHeight: 40
                border.width: 0
                color: root.defaultColor
            }
        }
        Rectangle {
            id: wildCardContainer
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: wildCardButtonPrevious.right
            anchors.right: wildCardButtonNext.left
            color: root.defaultColor

            ListView {
                id: wildCardView
                width: contentWidth
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                orientation: ListView.Horizontal
                interactive: false

                model: gameBackend.getWord()

                delegate: Item {
                    id: wildCardLetter
                    width: 30
                    Text {
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text: empty == "0" || hint == "1" ? letter : "_"
                        font.pixelSize: 36
                        color: hint == "1" ? root.hintColor : root.primaryColor
                    }
                }
            }
        }

    }

    Rectangle {
        anchors.bottom: parent.bottom
        anchors.top: wildCard.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 10

        ListView {
            anchors.fill: parent
            verticalLayoutDirection: ListView.BottomToTop
            interactive: false

            model: gameBackend.getFoundWords()
            delegate: Text {
                text: word
                color: wasInList == "1" ? root.primaryColor : root.secondaryColor
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

    Rectangle {
        id: gridContainer
        color: root.defaultColor
        anchors.horizontalCenter: parent.horizontalCenter
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
                width: 60
                height: 60
                Text {
                    anchors.fill: parent
                    id: gridLabel
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: letter
                    font.capitalization: Font.AllUppercase
                    font.pixelSize: 48
                    color: bloom == "0" ? root.secondaryColor : root.primaryColor
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
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        color: root.defaultColor

        Text {
            id: footerLabel
            text: "Erwan Bernard"
            anchors.rightMargin: 10
            anchors.leftMargin: 10
            anchors.fill: parent
            font.pixelSize: 12
            color: root.secondaryColor
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignRight
            minimumPointSize: 8
            minimumPixelSize: 8
        }
    }

    Rectangle {
        id: notificationContainer
        height: 30
        anchors.top: wildCard.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        color: root.notificationColor
        opacity: 0.0

        Text {
            anchors.fill: parent
            id: notificationLabel
            text: ""
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 24
            wrapMode: Text.WordWrap
        }

    }

    SequentialAnimation {
        id: allAnimation
        running: false

        ParallelAnimation {
            ColorAnimation { target: wildCard; property: "color"; from: root.defaultColor; to: root.notificationColor; duration: 10; easing.type: Easing.InCubic }
            NumberAnimation { target: notificationContainer; property: "opacity"; to: 1.0; duration: 10; easing.type: Easing.InCubic }
        }
        ParallelAnimation {
            NumberAnimation { target: notificationContainer; property: "opacity"; to: 0.0; duration: 400; easing.type: Easing.InCubic }
            ColorAnimation { target: wildCard; property: "color"; from: root.notificationColor; to: root.defaultColor; duration: 400; easing.type: Easing.InCubic }
        }
    }
}
