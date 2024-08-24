import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Basic as Basic
import Qt5Compat.GraphicalEffects

import "Constants.js" as Constants

TabBar {
    id: frame

    property bool hide_text: false

    padding: Constants.FRAME_PADDING

    signal pageChanged(int index)

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
            text_color: palette.text
            text_visible: !frame.hide_text

            checked: index === 0
            text: modelData

            onClicked: frame.actionForButton(modelData)

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
        text_color: palette.text
        text_visible: !frame.hide_text
        bg_color: "red"

        property bool loggedIn: false
        property string msg: loggedIn ? "Log Out" : "Log In"
        property string context: loggedIn ? "LogOut" : "LogIn"

        display: AbstractButton.TextUnderIcon
        source : `qrc/../assets/${log_button.context}.png`
        text: log_button.msg

        onClicked: frame.actionForButton(context)
    }

    ButtonGroup {
        id: button_group
        exclusive: true
    }

    function actionForButton(button: string): void {
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

    component SideElement: Basic.TabButton  {
        id: control
        text: qsTr("CheckDelegate")
        property url source: "qrc/../assets/Help.png"
        property int column_padding: 5
        property int img_width: 30
        property font text_font: control.font
        property color text_color: palette.text
        property color bg_color: "transparent"
        property bool text_visible: true

        hoverEnabled: true
        autoExclusive: true

        ButtonGroup.group: button_group

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
            implicitHeight: 5
            width: control.width / 2
            x: control.rightPadding + control.availableWidth / 2 - width/ 2
            y: control.height - control.bottomPadding
            radius: 5
            color: control.checked  ? palette.active.accent : "transparent"
        }

        background: Rectangle {
            implicitWidth: 100
            implicitHeight: 60
            radius: 10
            color: control.down || control.checked || control.hovered  ? palette.active.highlight : control.bg_color
        }
    }
}
