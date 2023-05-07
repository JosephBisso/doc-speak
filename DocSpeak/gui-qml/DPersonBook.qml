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
                model: ["Name", "Phone", "Email", "Sex", "Last Record"]

                delegate: Label {
                    text: modelData
                    font: Constants.FONT_MEDIUM_BOLD
                    opacity: 1
                    color: Constants.TEXT_COLOR
                    elide: Text.ElideRight
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.Wrap
                    Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
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
                ListElement{name: "name1"; phone: "phone1"; email: "email1"; sex: "sex1"; last_record: "last_record1"}
                ListElement{name: "name2"; phone: "phone2"; email: "email2"; sex: "sex2"; last_record: "last_record2"}
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
                        text: name
                        font: Constants.FONT_MEDIUM
                        opacity: 1
                        color: Constants.TEXT_COLOR
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.Wrap
                    }
                    Text {
                        Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                        text: phone
                        font: Constants.FONT_MEDIUM
                        opacity: 1
                        color: Constants.TEXT_COLOR
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.Wrap
                    }
                    Text {
                        Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                        text: email
                        font: Constants.FONT_MEDIUM
                        opacity: 1
                        color: Constants.TEXT_COLOR
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.Wrap
                    }
                    Text {
                        Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                        text: sex
                        font: Constants.FONT_MEDIUM
                        opacity: 1
                        color: Constants.TEXT_COLOR
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.Wrap
                    }
                    Text {
                        Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                        text: last_record
                        font: Constants.FONT_MEDIUM_BOLD
                        opacity: 1
                        color: Constants.ACCENT_COLOR
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.Wrap

                        MouseArea {
                            id: last_record_ma
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor 

                            onClicked: {console.log("record", last_record, "for", name, "clicked")}
                        }
                    }

                    // Rectangle {
                    //     id: email_button
                    //     Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                    //     Layout.fillHeight: true
                    //     Image {
                    //         id: email_button_img
                    //         anchors.centerIn: parent
                    //         width: Constants.IMAGE_SIZE_BIG; height: width
                    //         fillMode: Image.PreserveAspectFit
                    //         horizontalAlignment: Image.AlignHCenter
                    //         verticalAlignment: Image.AlignVCenter
                    //         source: "qrc/../assets/email.png"
                    //     }
                    //     ColorOverlay {
                    //         anchors.fill: email_button_img
                    //         source: email_button_img
                    //         color: Constants.TEXT_COLOR
                    //     }
                    //     MouseArea {
                    //         anchors.fill: parent
                    //         hoverEnabled: true
                    //         cursorShape: Qt.PointingHandCursor 

                    //         onClicked: {console.log("emailing", email)}

                    //     }
                    // }

                }
            }
        }
    }
}