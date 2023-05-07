import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

import "qrc:/Constants.js" as Constants

Frame {
    id: frame

    anchors.fill: parent

    background: Rectangle {
        anchors.fill: parent
        color: "transparent"
        border.color: "transparent"
    }

    DSearchBar {
        id: search_bar

        anchors {
            top: parent.top
            right: parent.right

            margins: 10
        }

        onFilter: (filterText) => {console.log("filtering for", filterText)}
    }


    ScrollView {
        id: scroll_view

        anchors {
            top: search_bar.bottom
            right: parent.right
            left: parent.left
            bottom: parent.bottom
        }

        clip: true
        ScrollBar.horizontal.policy: ScrollBar.AsNeeded
        ScrollBar.vertical.policy: ScrollBar.AlwaysOn

        RowLayout {
            id: list_title

            height: 50

            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
            }

            Repeater {
                model: ["Patient Name", "Doctor Name", "Date"]

                delegate: Label {
                    text: modelData
                    font: Constants.FONT_MEDIUM_BOLD
                    opacity: 1
                    color: Constants.TEXT_COLOR
                    elide: Text.ElideRight
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.Wrap
                    Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                    Layout.fillWidth: true
                }
            }
        }

        ListView {
            id: list_view

            spacing: 20

            anchors {
                left: parent.left
                right: parent.right
                top: list_title.bottom
                bottom: parent.bottom
            }

            model: ListModel {
                ListElement{patient_name: "patient name1"; doctor_name: "doctor name1"; date: "date1"}
                ListElement{patient_name: "patient name2"; doctor_name: "doctor name2"; date: "date2"}
            }

            delegate: Rectangle {
                id: rect_delegate

                width: list_view.width
                height: 60
                color: mouse_area.containsMouse ? Constants.ACCENT_2_COLOR  : Constants.BACKGROUND_COLOR
                radius: Constants.RECT_RADIUS

                MouseArea {
                    id: mouse_area
                    anchors.fill: parent
                    hoverEnabled: true

                    onClicked: {console.log(name, "clicked")}
                }

                RowLayout {
                    anchors.fill: parent
                    implicitWidth: list_view.width

                    Text {
                        Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                        Layout.fillWidth: true
                        text: patient_name
                        font: Constants.FONT_MEDIUM
                        opacity: 1
                        color: Constants.TEXT_COLOR
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.Wrap
                    }
                    Text {
                        Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                        Layout.fillWidth: true
                        text: doctor_name
                        font: Constants.FONT_MEDIUM
                        opacity: 1
                        color: Constants.TEXT_COLOR
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.Wrap
                    }
                    Text {
                        Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                        Layout.fillWidth: true
                        text: date
                        font: Constants.FONT_MEDIUM
                        opacity: 1
                        color: Constants.TEXT_COLOR
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.Wrap
                    }
                }
            }
        }
    }
}