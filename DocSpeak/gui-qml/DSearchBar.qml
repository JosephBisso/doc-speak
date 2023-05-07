import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

import "qrc:/Constants.js" as Constants

Frame {
    id: rootSearchBar
    property real mRadius: Constants.RECT_RADIUS

    width: 250
    height: Math.min(50, width / 2)
    opacity: 0.4

    background: Rectangle {
        anchors.fill: parent
        radius: rootSearchBar.mRadius
        color: "white"
        border {
            color: "transparent"
        }
    }
    signal filter(string filterText)

    Timer {
        id: fadeTimer
        interval: 500

        onTriggered: {
            rootSearchBar.opacity = 0.4
        }
    }
    Behavior on opacity {NumberAnimation {duration: 150}}

    RowLayout {
        clip: true
        anchors.fill: parent
        Item {
            implicitWidth: searchImg.width
            implicitHeight: searchImg.height
            Image {
                id: searchImg
                source: "qrc/../assets/loupe.png"
                sourceSize: Qt.size(rootSearchBar.height * 0.5 , rootSearchBar.height * 0.5 )
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                Layout.leftMargin: rootSearchBar.mRadius * 0.25
            }

            ColorOverlay {
                id: fileOverlay
                source: searchImg
                anchors.fill: searchImg
                color: "black"
            }
        }

        TextField {
            id: searchText
            placeholderText: "Search..."
            font: Constants.FONT_SMALL
            color: "black"
            background: null
            hoverEnabled: true
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
            Layout.fillWidth: true
            onTextEdited: rootSearchBar.filter(text)
            onHoveredChanged: {
                if(hovered) {
                    rootSearchBar.opacity = 1
                    fadeTimer.stop()
                } else {
                    fadeTimer.start()
                }
            }
        }
    }
}
