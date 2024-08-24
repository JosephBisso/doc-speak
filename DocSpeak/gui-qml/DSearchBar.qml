import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic as Basic
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

import "Constants.js" as Constants

Basic.Frame {
    id: rootSearchBar
    property real mRadius: Constants.RECT_RADIUS

    width: 250
    height: Math.min(50, width / 2)
    opacity: 0.4

    background: Rectangle {
        radius: rootSearchBar.height
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
                color: searchText.color
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
