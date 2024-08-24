import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

import "Constants.js" as Constants

ToolBar {
        id: header_bar_frame
        property string title: "Title"

        // property int column_padding: 5
        property int img_width: 30
        property font text_font
        property font page_font: text_font
        property color text_color: palette.text

        function setUserName (user_name : string): void {
            user_name.text = user_name
        }

        padding: 2
        height: 40

        RowLayout {
            anchors.fill: parent

            Image {
                id: app_button
                fillMode: Image.PreserveAspectFit
                source: "qrc/../assets/pharmacy.png"
                verticalAlignment: Image.AlignVCenter

                Layout.alignment: Qt.AlignVCenter
                Layout.preferredWidth: header_bar_frame.img_width
                Layout.preferredHeight: header_bar_frame.img_width

                Layout.leftMargin: 5
            }

            Text {
                id: windowTitle
                text: header_bar_frame.title
                color: palette.text
                font: Constants.FONT_MEDIUM_SPECIAL
                verticalAlignment: Text.AlignVCenter

                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
            }

            Text {
                id: user_name
                text: "Herr Prof. Dr. Max MusterMann"
                color: palette.text
                font: header_bar_frame.page_font
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignCenter
            }

            ToolButton {
                id: notification_rect

                Layout.alignment: Qt.AlignVCenter
                Layout.fillHeight: true
                Layout.preferredWidth: header_bar_frame.height
                // Layout.margins: 2
                hoverEnabled: true

                background: Rectangle {
                    // implicitWidth: 100
                    // implicitHeight: 60
                    radius: 10
                    color: notification_rect.hovered ? palette.highlight : "transparent"
                }

                contentItem: Item {
                    Image {
                        id: notification
                        anchors.centerIn: parent
                        width: 30
                        height: width
                        fillMode: Image.PreserveAspectFit
                        source: "qrc/../assets/notification.png"
                        verticalAlignment: Image.AlignVCenter
                    }

                    ColorOverlay {
                        anchors.fill: notification
                        source: notification
                        color: palette.text
                    }
                }

                onClicked: {
                    console.log("Notification clicked")
                }
            }
        }
    }
