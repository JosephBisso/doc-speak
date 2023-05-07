import QtQml 2.15
import QtQuick 2.15
import Qt.labs.settings 1.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQml.Models 2.15
import QtGraphicalEffects 1.15

import "Constants.js" as Constants

ApplicationWindow {
    id: rootWindow
    flags: Qt.Window | Qt.FramelessWindowHint
    x: windowsSettings.value("rootX", 0)
    y: windowsSettings.value("rootY", 0)
    width: windowsSettings.value("rootWidth", 960)
    height: windowsSettings.value("rootHeight", 720)

    minimumWidth: 960
    minimumHeight: 720
    visible: true

    background: Rectangle {
        anchors.fill: parent
        color: Constants.BACKGROUND_COLOR
    }

    Component.onCompleted: {
    }

    Rectangle {
        id: rect_sizable
        anchors.fill: parent
        color: "transparent"
        property point mPos: Qt.point(0,0)

        MouseArea {
            anchors.fill: parent
            
            hoverEnabled: true
            acceptedButtons: Qt.LeftButton

            property int edges: 0;
            property int edgeOffest: 5;

            function setEdges(x, y) {
                edges = 0;
                if(x < edgeOffest) edges |= Qt.LeftEdge;
                if(x > (width - edgeOffest))  edges |= Qt.RightEdge;
                if(y < edgeOffest) edges |= Qt.TopEdge;
                if(y > (height - edgeOffest)) edges |= Qt.BottomEdge;
            }

            cursorShape: {
                return !containsMouse ? Qt.ArrowCursor:
                    edges == 3 || edges == 12 ? Qt.SizeFDiagCursor :
                    edges == 5 || edges == 10 ? Qt.SizeBDiagCursor :
                    edges & 9 ? Qt.SizeVerCursor :
                    edges & 6 ? Qt.SizeHorCursor : Qt.ArrowCursor;
            }

            onPositionChanged: setEdges(mouseX, mouseY);
            onPressed: {
                setEdges(mouseX, mouseY);
                if(edges && containsMouse) {
                    rootWindow.startSystemResize(edges);
                }
            }
        }
    }

    TitleBar {
        id: title_bar
        title: qsTr("DocSpeak")

        text_font: Constants.FONT_SMALL
        text_color: Constants.TEXT_ACCENT_COLOR
        img_width: Constants.IMAGE_SIZE_SMALL

        anchors.margins: 2
    }

    DSideBar {
        id: sidebar
        anchors.top: title_bar.bottom
        anchors.margins: 2
    }

    DMainView {
        id: main_view

        anchors.left: sidebar.right
        anchors.top: title_bar.bottom
        anchors.margins: 2
    }

    Component.onDestruction: {
        console.info("Saving Configurations for Root Window ...")
        windowsSettings.setValue("rootX", rootWindow.x)
        windowsSettings.setValue("rootY", rootWindow.y)
        windowsSettings.setValue("rootWidth", rootWindow.width)
        windowsSettings.setValue("rootHeight", rootWindow.height)
    }

    Settings {
        id: windowsSettings
        category: "windows"
    }

    component TitleBar : Frame {
        id: title_bar_frame
        property string title: "Title"

        // property int column_padding: 5
        property int img_width: 30
        property font text_font
        property color text_color: "black"

        property point mPos: Qt.point(0,0)

        padding: 2
        height: 40


        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        background: Rectangle {
            anchors.fill: parent
            color: "transparent"
            border.color: "transparent"
        }

        MouseArea {
            anchors.fill: parent
            onPressed: {
                title_bar_frame.mPos = Qt.point(mouseX, mouseY)
            }
            onPositionChanged: {
                if (rootWindow.visibility == 4 || rootWindow.visibility == 5) { // if Maximized
                    rootWindow.visibility = 1  // Automatic visibility
                }
                rootWindow.setX(rootWindow.x + mouseX - title_bar_frame.mPos.x)
                rootWindow.setY(rootWindow.y + mouseY - title_bar_frame.mPos.y)
            }
        }

        RowLayout {
            anchors.fill: parent

            Image {
                id: app_button
                fillMode: Image.PreserveAspectFit
                source: "qrc/../assets/pharmacy.png"
                verticalAlignment: Image.AlignVCenter

                Layout.alignment: Qt.AlignVCenter
                Layout.preferredWidth: title_bar_frame.img_width
                Layout.preferredHeight: title_bar_frame.img_width

                Layout.leftMargin: 5
            }

            Text {
                id: windowTitle
                text: title_bar_frame.title
                color: title_bar_frame.text_color
                font: title_bar_frame.text_font
                verticalAlignment: Text.AlignVCenter

                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                // Layout.preferredWidth: 30
                // Layout.preferredHeight: 30
            }

            Rectangle {
                id: notification_rect
                color: notification_ma.containsMouse ? "grey" : "transparent"

                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                Layout.fillHeight: true
                Layout.preferredWidth: title_bar_frame.height
                // Layout.margins: 2
                radius: 10

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
                    color: title_bar_frame.text_color
                }

                MouseArea {
                    id: notification_ma
                    cursorShape: Qt.PointingHandCursor 
                    anchors.fill: notification_rect
                    hoverEnabled: true
                    onClicked: {
                        console.log("Notification clicked")
                    }
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                Layout.fillHeight: true
                spacing: 15

                Repeater {
                    model: ListModel {
                        ListElement {name: "minimize" ; highlight: "gray"}
                        ListElement {name: "maximize"; highlight: "gray"}
                        ListElement {name: "close"; highlight: "red"}
                    }

                    delegate: Rectangle {
                        id: delegate_rect
                        color: mouse_area.containsMouse ? highlight : "transparent"

                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                        Layout.fillHeight: true
                        Layout.preferredWidth: title_bar_frame.height
                        Layout.maximumWidth: title_bar_frame.height + 1

                        Image {
                            id: img_delegate
                            anchors.centerIn: parent
                            width: title_bar_frame.img_width - 5
                            height: width
                            fillMode: Image.PreserveAspectFit
                            source: `qrc/../assets/${name}.png`
                            verticalAlignment: Image.AlignVCenter
                        }

                        ColorOverlay {
                            anchors.fill: img_delegate
                            source: img_delegate
                            color: title_bar_frame.text_color
                        }

                        MouseArea {
                            id: mouse_area
                            anchors.fill: delegate_rect
                            hoverEnabled: true
                            onClicked: {
                                console.log(`${name} clicked`)
                                switch(name) {
                                    case "minimize": 
                                        rootWindow.showMinimized()
                                        break;
                                    case "maximize": 
                                        console.log(rootWindow.visibility)
                                        if (rootWindow.visibility == 4 || rootWindow.visibility == 5) { // if Maximized
                                            rootWindow.visibility = 1  // Automatic visibility
                                        } else {
                                            rootWindow.showMaximized()
                                        }
                                        console.log(rootWindow.visibility)
                                        break;
                                    case "close": 
                                        rootWindow.close()
                                        break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}