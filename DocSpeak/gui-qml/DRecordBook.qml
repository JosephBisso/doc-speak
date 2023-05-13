import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

import "qrc:/Constants.js" as Constants

Frame {
    id: frame

    property ListModel all_records: ListModel{
        ListElement{patient_name: "patient name1"; doctor_name: "doctor name1"; date: "date1"}
        ListElement{patient_name: "patient name2"; doctor_name: "doctor name2"; date: "date2"}
    }


    anchors.fill: parent

    background: Rectangle {
        anchors.fill: parent
        color: "transparent"
        border.color: "transparent"
    }

    function searchElement(text) {
        view_list_model.clear()
        for (let i = 0; i < all_records.count; i++) {
            if (all_records.get(i).patient_name.match(text) || 
                all_records.get(i).doctor_name.match(text) ||
                all_records.get(i).date.match(text) 
            ) {
                view_list_model.append(
                    {
                        "patient_name"      : all_records.get(i).patient_name,
                        "doctor_name"     : all_records.get(i).doctor_name,
                        "date"     : all_records.get(i).date,
                    }
                )
            }
        }

        list_view.model = view_list_model
    }

    Component.onCompleted: {list_view.model = frame.all_records}


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
                list_view.model = frame.all_records
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
                color: mouse_area.containsMouse ? Constants.ACCENT_2_COLOR  : Constants.BACKGROUND_COLOR
                radius: Constants.RECT_RADIUS

                MouseArea {
                    id: mouse_area
                    anchors.fill: parent
                    hoverEnabled: true

                    onClicked: {console.log("Clicked:", patient_name, doctor_name, date)}
                }

                RowLayout {
                    anchors.fill: parent

                    Repeater {
                        model: [patient_name, doctor_name, date]

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