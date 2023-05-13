import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 1.4 as C1
import Qt.labs.calendar 1.0

import "qrc:/Constants.js" as Constants

Frame {
    id: frame

    property var _locale: Qt.locale()
    property date currentDate: new Date()
    property ListModel all_dates: ListModel {
        ListElement {start: "2023-05-09T10:56:06"; end: "2023-05-09T11:56:06"; msg: "Record1, Patient1, Doctor1"}
        ListElement {start: "2023-05-12T12:56:06"; end: "2023-05-12T13:56:06"; msg: "Record2, Patient2, Doctor2"}
        ListElement {start: "2023-05-12T14:56:06"; end: "2023-05-12T16:56:06"; msg: "Record3, Patient3, Doctor3"}
        ListElement {start: "2023-05-12T16:56:06"; end: "2023-05-12T17:56:06"; msg: "Record3, Patient3, Doctor3"}
    }

    anchors.fill: parent

    background: Rectangle {
        anchors.fill: parent
        color: "transparent"
        border.color: "transparent"
    }

    function check_if_day_in_list (target_date) {
        
        for (let i = 0; i < all_dates.count; i++) {
            let date = new Date(all_dates.get(i).start)

             if (date.getMonth() === target_date.month && 
                 date.getFullYear() === target_date.year &&
                 date.getDate() === target_date.day) 
             {
                console.log("element in list", all_dates.get(i).start, date.getMonth(), target_date.year, target_date.month, target_date.day)
                return true
             }
         }
         return false
    }

    RowLayout {
        id: row_layout
        anchors.fill: parent
        spacing: 2 * Constants.FRAME_PADDING

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true

            color: "transparent"
            radius: Constants.RECT_RADIUS

            RowLayout {
                id: calendar_title_row
                width: parent.width
                height: 40
                anchors {
                    top: parent.top
                    horizontalCenter: parent.horizontalCenter
                }

                Button {
                    id: calendar_back_button
                    text: "<"
                    onClicked: {listview.decrementCurrentIndex()}
                    hoverEnabled: true

                    Layout.alignment: Qt.AlignRight
                    background: Rectangle {
                        anchors.fill: parent
                        color: calendar_back_button.hovered ? Constants.ACCENT_COLOR : "transparent"
                        radius: Constants.RECT_RADIUS
                    }

                    contentItem: Text {
                        text: calendar_back_button.text
                        font: Constants.FONT_MEDIUM_BOLD
                        opacity: 1
                        color: Constants.TEXT_COLOR
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.Wrap
                    }
                }

                Label {
                    id: calendar_title
                    text: `${frame._locale.monthName(listview.model.monthAt(listview.currentIndex))} ${listview.model.yearAt(listview.currentIndex)}`
                    font: Constants.FONT_MEDIUM_BOLD
                    opacity: 1
                    color: Constants.TEXT_COLOR
                    elide: Text.ElideRight
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.Wrap
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                }

                Button {
                    id: calendar_next_button
                    text: ">"
                    onClicked: {listview.incrementCurrentIndex()}
                    hoverEnabled: true

                    Layout.alignment: Qt.AlignLeft

                    background: Rectangle {
                        anchors.fill: parent
                        color: calendar_next_button.hovered ? Constants.ACCENT_COLOR : "transparent"
                        radius: Constants.RECT_RADIUS
                    }

                    contentItem: Text {
                        text: calendar_next_button.text
                        font: Constants.FONT_MEDIUM_BOLD
                        opacity: 1
                        color: Constants.TEXT_COLOR
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.Wrap
                    }
                }
            }

            DayOfWeekRow {
                id: days_row
                
                width: parent.width

                anchors {
                    top: calendar_title_row.bottom
                    left: listview.left
                    right: listview.right

                    topMargin: 2 * Constants.FRAME_PADDING
                }
                delegate: Text {
                    text: model.shortName
                    font: Constants.FONT_MEDIUM_BOLD
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: Constants.TEXT_COLOR
                }
            }

            ListView {
                id: listview
                
                width: parent.width

                anchors {
                    left: parent.left
                    right: parent.right
                    top: days_row.bottom
                    bottom: parent.bottom

                    margins: Constants.FRAME_PADDING
                }
                
                snapMode: ListView.SnapOneItem
                orientation: ListView.Horizontal
                highlightRangeMode: ListView.StrictlyEnforceRange
                clip: true

                Component.onCompleted: {
                    let date = new Date()
                    let today_index = listview.model.indexOf(date.getFullYear(), date.getMonth())
                    if (today_index > -1) {
                        console.log("index", today_index, "of", listview.model.count, "date:", date.getFullYear(), date.getMonth())
                        console.log("Check: index", today_index, listview.itemAtIndex(today_index),listview.model.yearAt(today_index))
                        listview.positionViewAtIndex(today_index, ListView.Center)
                        listview.currentIndex = today_index
                    }
                }

                model: CalendarModel {
                    from: new Date(2022, 0, 1)
                    to: {
                        let today = new Date()
                        console.log("Actual Year", today.getFullYear())
                        return new Date(today.getFullYear() + 5, 0, 1)
                    }
                }

                delegate: MonthGrid {
                    id: month_grid
                    width: listview.width
                    height: listview.height

                    month: model.month
                    year: model.year

                    delegate: Rectangle {
                        id: rect_delegate
                        property color _color: Constants.ACCENT_COLOR
                        property date _date: new Date(model.year, model.month, model.day)
                        height: 20
                        width: 20
                        radius: Constants.RECT_RADIUS
                        color: {
                            if (frame.currentDate.getMonth() === model.month && 
                                frame.currentDate.getFullYear() === model.year &&
                                frame.currentDate.getDate() === model.day) 
                            {
                                console.log("Calendar day at", frame.currentDate.getMonth(), frame.currentDate.getFullYear(), frame.currentDate.getDate())
                                time_plan.displayElementsFor(frame.currentDate)
                                return Constants.transparentBy(_color, 0.4)
                            }
                            return mouse_area.containsMouse ? Constants.transparentBy(_color, 0.1) : "transparent"
                        }
                        border.width: 4
                        border.color: {
                            if (frame.check_if_day_in_list(model)) {
                                return Constants.randomColor()
                            }
                            return  Constants.transparentBy(Constants.TEXT_COLOR, 0.1)
                        }

                        MouseArea {
                            id: mouse_area
                            hoverEnabled: true   
                            anchors.fill: parent
                            onClicked: {
                                time_plan._color = rect_delegate.border.color
                                time_plan.displayElementsFor(rect_delegate._date)
                            }
                        }

                        Text {
                            anchors.centerIn: parent
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            opacity: model.month === month_grid.month ? 1 : 0.3
                            text: model.day
                            font: Constants.FONT_MEDIUM
                            color: Constants.TEXT_COLOR
                        }
                    }
                }

                ScrollIndicator.horizontal: ScrollIndicator { }
            }
        }
        
        Rectangle {
            id: time_plan

            property color _color: Constants.ACCENT_COLOR

            width: 350
            Layout.fillHeight: true
            color: Constants.transparentBy(_color, 0.2)
            radius: Constants.RECT_RADIUS

            function displayElementsFor (clickedDate) {
                console.log("Displaying Element for date", clickedDate)
                day_record_list.clear()
                title.day = `${frame._locale.dayName(clickedDate.getDay())} ${clickedDate.getDate()}/${clickedDate.getMonth()}/${clickedDate.getFullYear()}`
                for (let i = 0; i < all_dates.count; i++) {
                    let date = new Date(all_dates.get(i).start)

                    if (date.getMonth() === clickedDate.getMonth() && 
                        date.getFullYear() === clickedDate.getFullYear() &&
                        date.getDate() === clickedDate.getDate()) 
                    {
                        
                        day_record_list.append({
                            start   : `${(new Date(all_dates.get(i).start)).getHours()}:${(new Date(all_dates.get(i).start)).getMinutes()}`,
                            end     : `${(new Date(all_dates.get(i).end)).getHours()}:${(new Date(all_dates.get(i).end)).getMinutes()}`,
                            msg     : all_dates.get(i).msg
                        })
                    }
                }
                
            }

            Label {
                id: title
                anchors {
                    top: parent.top
                    horizontalCenter: parent.horizontalCenter

                    topMargin:  Constants.FRAME_PADDING
                }
                property string day: "Today"
                text: `Events on ${title.day}`
                font: Constants.FONT_MEDIUM_BOLD
                opacity: 1
                color: Constants.TEXT_COLOR
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.Wrap
            }

            ScrollView {
                id: scroll_view

                
                clip: true
                ScrollBar.horizontal.policy: ScrollBar.AsNeeded
                ScrollBar.vertical.policy: ScrollBar.AsNeeded

                anchors {
                    left: parent.left
                    right: parent.right
                    top: title.bottom
                    bottom: parent.bottom

                    margins: 2 * Constants.FRAME_PADDING
                    topMargin: 25
                }

                ListView {
                    id: list_view

                    spacing: 20
                    anchors.fill: parent

                    model: ListModel {id: day_record_list}

                    delegate: GridLayout {
                        flow: GridLayout.TopToBottom
                        width: list_view.width - Constants.FRAME_PADDING
                        height: 100
                        columns: 2
                        rows: 2
                        rowSpacing: 0
                        columnSpacing: 0

                        Repeater {
                            model: [start, end]
                            delegate: Rectangle {
                                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                color: "transparent"
                                border.color: "transparent"

                                Text {
                                    anchors.centerIn: parent
                                    text: modelData
                                    font: Constants.FONT_SMALL_BOLD
                                    opacity: 1
                                    color: Constants.TEXT_COLOR
                                    elide: Text.ElideRight
                                    verticalAlignment: Text.AlignVCenter
                                    wrapMode: Text.Wrap
                                }
                            }
                        }

                        Rectangle {
                            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            Layout.rowSpan: 2
                            color: Constants.transparentBy(Constants.randomColor(), 0.3)
                            radius: Constants.RECT_RADIUS
                            border {
                                width: 2
                                color: "transparent"
                            }

                            Text {
                                anchors {
                                    fill: parent
                                    margins: 2
                                }
                                text: msg
                                font: Constants.FONT_SMALL
                                opacity: 1
                                color: Constants.TEXT_COLOR
                                elide: Text.ElideRight
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                wrapMode: Text.Wrap
                            }
                        }
                        
                    }
                    
                }
            }

        }
         
    }

}