import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

import "qrc:/Constants.js" as Constants

Frame {
    id: frame

    property bool hide_text: false

    padding: Constants.FRAME_PADDING
    width: 90

    
    signal pageChanged(int index)
    
    anchors {
        left: parent.left
        bottom: parent.bottom
    }

    background: Rectangle {
        anchors.fill: parent
        color: "transparent"
    }

    Behavior on width {NumberAnimation{duration: 100; easing.type: Easing.InOutQuad}}

    Timer {
        id: text_hide_timer
        interval: 3000; running: true; repeat: false; triggeredOnStart: false
        onTriggered: {
            console.log("fadding side bar")
            frame.hide_text = true
            frame.width = 60
        }

        function show_text() {
            frame.width = 90
            frame.hide_text = false
            text_hide_timer.stop()
        }

        function start_hide_timer() {
            text_hide_timer.restart()
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

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
                text_visible: !frame.hide_text

                checked: index == 0
                text: modelData

                onClicked: frame.actionForButton(modelData)
                onHoveredChanged: {
                    if (hovered) {
                        text_hide_timer.show_text()
                    } else {
                        text_hide_timer.start_hide_timer()
                    }
                }
                
                Layout.fillWidth: true
                Layout.fillHeight: true
                
                ButtonGroup.group: button_group
            }
        }

        SideElement  {
            id: log_button

            column_padding: Constants.FRAME_PADDING
            img_width: Constants.IMAGE_SIZE_BIG
            text_font: Constants.FONT_VERY_SMALL
            text_color: Constants.TEXT_ACCENT_COLOR
            text_visible: !frame.hide_text

            property bool loggedIn: false
            property string msg: loggedIn ? "Log Out" : "Log In"
            property string context: loggedIn ? "LogOut" : "LogIn"

            display: AbstractButton.TextUnderIcon
            source : `qrc/../assets/${log_button.context}.png`
            text: log_button.msg

            onClicked: frame.actionForButton(context)
            onHoveredChanged: {
                 if (hovered) {
                    text_hide_timer.show_text()
                } else {
                    text_hide_timer.start_hide_timer()
                }
            }
            
            Layout.fillWidth: true
            Layout.fillHeight: true

            ButtonGroup.group: button_group
        }
    }

    ButtonGroup {
        id: button_group
        exclusive: true
    }

    function actionForButton(button) {
        console.log(`${button} clicked`)
        switch(button) {
            case "Dashboard":
                frame.pageChanged(0)
                break;
            case "Doctors":
                frame.pageChanged(1)
                break;
            case "Patients":
                frame.pageChanged(2)
                break;
            case "Calendar":
                frame.pageChanged(3)
                break;
            case "Report":
                frame.pageChanged(4)
                break;
            case "Settings":
                frame.pageChanged(5)
                break;
            case "Help":
                frame.pageChanged(6)
                break;
            case "LogIn":
            case "LogOut":
                frame.pageChanged(7)
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
        property bool text_visible: true

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
                visible: control.text_visible
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