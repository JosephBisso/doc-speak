import QtQml 2.15
import QtQuick 2.15
import Qt.labs.settings 1.0
import QtQuick.Controls 2.15

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
        color: "cyan"
    }

    Component.onCompleted: {
    }

    TitleBar {
        id: title_bar
        title: qsTr("DocSpeak")
    }

    DSideBar {
        id: sidebar
        anchors.top: title_bar.bottom

    }

    DMainView {
        id: main_view

        anchors.left: sidebar.right
        anchors.top: title_bar.bottom
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
        property string title: "Title"

        id: frame
        width: parent.width
        height: 30

        property point mPos: Qt.point(0,0)

        anchors {
            top: parent.top
            left:parent.left
        }

        background: Rectangle {
            color: "transparent"
            border.color: "transparent"
        }

        MouseArea {
            anchors.fill: parent
            onPressed: {
                mPos = Qt.point(mouseX, mouseY)
            }
            onPositionChanged: {
                rootWindow.setX(rootWindow.x + mouseX - mPos.x)
                rootWindow.setY(rootWindow.y + mouseY - mPos.y)
            }
        }

        Row {
            anchors.fill: parent
            layoutDirection: Qt.LeftToRight

            Text {
                id: windowTitle
                text: title
                color: "black"
                font.pointSize: 12
            }

            Image {
                id: closebutton
                width: 22
                height: 22
                fillMode: Image.PreserveAspectFit
                source: "qrc/../assets/close.png"

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        Qt.quit()
                    }
                }
            }

        }

    }
}