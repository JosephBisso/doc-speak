import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Frame {
    id: frame

    padding: 10
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
        color: "black"
        radius: frame.padding
    }

    Rectangle {
        anchors.fill: parent
        
        color: "transparent"
        border {
            width: 5
            color: "cyan"
        }
    }
}