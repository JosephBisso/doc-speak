import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

import "Constants.js" as Constants

Frame {
    id: frame

    padding: Constants.FRAME_PADDING
    width: 90

    anchors {
        left: parent.left
        bottom: parent.bottom
    }

    background: Rectangle {
        anchors.fill: parent
        color: "transparent"
    }

    ColumnLayout {
        anchors.fill: parent
        
        Repeater {
            model: [
                "Dashboard",
                "Doctors",
                "Patients",
                "Calendar",
                "Report",
                "Settings",
                "Help"
            ]

            delegate: SideElement  {
                display: AbstractButton.TextUnderIcon
                source : `qrc/../assets/${modelData}.png`
                column_padding: Constants.FRAME_PADDING
                img_width: Constants.IMAGE_SIZE_BIG
                text_font: Constants.FONT_VERY_SMALL
                text_color: Constants.TEXT_ACCENT_COLOR

                text: modelData

                onClicked: frame.actionForButton(modelData)
                
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }

        SideElement  {
            id: log_button

            column_padding: Constants.FRAME_PADDING
            img_width: Constants.IMAGE_SIZE_BIG
            text_font: Constants.FONT_VERY_SMALL
            text_color: Constants.TEXT_ACCENT_COLOR

            property bool loggedIn: false
            property string msg: loggedIn ? "Log Out" : "Log In"
            property string context: loggedIn ? "LogOut" : "LogIn"

            display: AbstractButton.TextUnderIcon
            source : `qrc/../assets/${log_button.context}.png`
            text: log_button.msg

            onClicked: frame.actionForButton(context)
            
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    function actionForButton(button) {
        console.log(`${button} clicked`)
        switch(button) {
            case "Dashboard":
                break;
            case "Doctors":
                break;
            case "Patients":
                break;
            case "Calendar":
                break;
            case "Report":
                break;
            case "Settings":
                break;
            case "Help":
                break;
            case "LogIn":
                break;
            case "LogOut":
                break;
        }
    }

    component SideElement: CheckDelegate  {
        id: control
        text: qsTr("CheckDelegate")
        property url source: "qrc/../assets/Help.png"
        property int column_padding: 5
        property int img_width: 30
        property font text_font: control.font
        property color text_color: "black"

        hoverEnabled: true

        contentItem: ColumnLayout {
            id: column
            anchors.centerIn: control
            Item {
                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                Layout.fillWidth: true
                Layout.fillHeight: true
                Image {
                    id: delegate_img
                    anchors.centerIn: parent
                    width: control.img_width; height: width
                    fillMode: Image.PreserveAspectFit
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: control.source
                }
                ColorOverlay {
                    anchors.fill: delegate_img
                    source: delegate_img
                    color: control.text_color
                }

            }
            Text {
                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                Layout.fillWidth: true
                text: control.text
                font: control.text_font
                opacity: 1
                color: control.text_color
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.Wrap
            }
        }

        indicator: Rectangle {
            implicitWidth: 5
            height: control.height / 2
            x: 0
            y: control.topPadding + control.availableHeight / 2 - height / 2
            radius: 5
            color: control.down || control.checked  ? "blue" : "transparent"
        }

        background: Rectangle {
            implicitWidth: 100
            implicitHeight: 40
            radius: 10
            color: {
                if (control.down) {return "skyblue";}
                else if (control.hovered) {return "lightblue";}
                else if (control.checked) {return "skyblue";}
                else {return "transparent";}
            }
        }
    }
}