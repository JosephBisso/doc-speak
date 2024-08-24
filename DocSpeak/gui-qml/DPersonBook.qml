import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "Constants.js" as Constants

Frame {
    id: frame

    property ListModel all_persons: ListModel{
        ListElement{name: "name1"; phone: "phone1"; email: "email1"; sex: "sex1"; last_record: "last_record1"}
        ListElement{name: "name2"; phone: "phone2"; email: "email2"; sex: "sex2"; last_record: "last_record2"}
    }

    anchors.fill: parent

    function searchElement(text) {
        view_list_model.clear()
        for (let i = 0; i < all_persons.count; i++) {
            if (all_persons.get(i).name.match(text) || 
                all_persons.get(i).phone.match(text) ||
                all_persons.get(i).email.match(text) ||
                all_persons.get(i).sex.match(text) ||
                all_persons.get(i).last_record.match(text)
            ) {
                view_list_model.append(
                    {
                        "name"      : all_persons.get(i).name,
                        "phone"     : all_persons.get(i).phone,
                        "email"     : all_persons.get(i).email,
                        "sex"       : all_persons.get(i).sex,
                        "last_record"  : all_persons.get(i).last_record
                    }
                )
            }
        }

        list_view.model = view_list_model
    }

    Component.onCompleted: {list_view.model = frame.all_persons}

    DSearchBar {
        id: search_bar

        anchors {
            top: parent.top
            right: parent.right

            margins: 10
        }

        onFilter: (filterText) => {
            console.log("filtering for", filterText)
            if (!filterText) {
                list_view.model = frame.all_persons
                return
            }
            frame.searchElement(filterText)
        }
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
        ScrollBar.vertical.policy: ScrollBar.AsNeeded

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

                delegate: Rectangle {
                    Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                    Layout.fillWidth: true
                    Label {
                        anchors.centerIn: parent
                        text: modelData
                        font: Constants.FONT_MEDIUM_BOLD
                        opacity: 1
                        color: Constants.TEXT_COLOR
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.Wrap
                    }
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

            model: ListModel {id: view_list_model}

            delegate: Rectangle {
                id: rect_delegate

                width: list_view.width
                height: 60
                color: mouse_area.containsMouse ? palette.active.accent  : palette.active.window
                radius: Constants.RECT_RADIUS

                MouseArea {
                    id: mouse_area
                    anchors.fill: parent
                    hoverEnabled: true

                    onClicked: {console.log("Clicked:", name, phone, email, sex, last_record)}
                }

                RowLayout {
                    anchors.fill: parent

                    Repeater {
                        model: [name, phone, email, sex, last_record]

                        delegate: Rectangle {
                            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                            Layout.fillWidth: true
                            Text {
                                anchors.centerIn: parent
                                text: modelData
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
    }
}
