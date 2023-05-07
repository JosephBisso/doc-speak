import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "Constants.js" as Constants

Frame {
    id: frame

    padding: Constants.FRAME_PADDING
    bottomPadding: 3*padding
    rightPadding: 3*padding

    anchors {
        right: parent.right
        bottom: parent.bottom

        rightMargin: -2*padding
        bottomMargin: -2*padding
    }


    background: Rectangle {
        anchors.fill: parent
        color: Constants.FOREGROUND_COLOR
        radius: frame.padding
    }

    Rectangle {
        anchors.fill: parent
        
        color: "transparent"
        border {
            width: 5
            color: "transparent"
        }
    }
}