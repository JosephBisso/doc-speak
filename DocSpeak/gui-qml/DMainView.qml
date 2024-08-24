import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "Constants.js" as Constants

Page {
    id: root_page

    padding: Constants.FRAME_PADDING
    bottomPadding: 3*padding
    rightPadding: 3*padding

    anchors.fill: parent
    clip: true

    property int page_index: 0

    property Component dashboard: Frame{}
    property Component doctor_book: DPersonBook{}
    property Component patient_book: DPersonBook{}
    property Component calendar: DCalendar{}
    property Component record_book: DRecordBook{}
    property Component log: Frame{}


    property var all_pages: [
        {page_name: "Dashboard", page_content: dashboard},
        {page_name: "Doctor Book", page_content: doctor_book},
        {page_name: "Patient Book", page_content: patient_book},
        {page_name: "Calendar", page_content: calendar},
        {page_name: "Record Book", page_content: record_book},
        {page_name: "Log In/Out", page_content: log}
    ]

    function loadPage(index : int) : void {
        root_page.page_index = index;
    }

    function pageLoaded(name: string) : void {
        page_header.header_title.text = name
    }

    function speak(): void {
        let visible = foot_tab_bar.foot_tab_button.visible
        foot_tab_bar.foot_tab_button.visible = !visible
    }

    header: ToolBar {
        id: page_header
        property alias header_title: page_name

        RowLayout {
            anchors.fill: parent
            ToolButton {
                font: Constants.FONT_SMALL
                text: qsTr("Speak")
                onClicked: console.log(`Speaking at Page ${page_name.text}`)
                Layout.alignment: Qt.AlignCenter
            }
            Label {
                id: page_name
                text: "Page Name"
                font: Constants.FONT_MEDIUM_BOLD
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                color: palette.text
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignCenter
            }
            ToolButton {
                text: qsTr("⋮")
                font: Constants.FONT_MEDIUM_BOLD
                onClicked: console.log(`More Infos for Page ${page_name.text}`)
                Layout.alignment: Qt.AlignCenter
            }
        }
    }

    Loader {
        id: loader
        active: true
        visible: status == Loader.Ready
        asynchronous: true

        readonly property var current_page: root_page.all_pages[root_page.page_index]

        onLoaded: {
            root_page.pageLoaded(current_page.page_name)
        }
        sourceComponent: current_page.page_content

        anchors.fill: parent

    }


}


