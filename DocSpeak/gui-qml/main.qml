import QtQml
import QtQuick
import QtCore
import QtQuick.Controls
import QtQuick.Layouts
import QtQml.Models


import "Constants.js" as Constants

ApplicationWindow {
    id: rootWindow
    flags: Qt.Window
    x: windowsSettings.value("rootX", 0)
    y: windowsSettings.value("rootY", 0)
    width: windowsSettings.value("rootWidth", 960)
    height: windowsSettings.value("rootHeight", 720)

    minimumWidth: 960
    minimumHeight: 720
    visible: true

    readonly property Palette dark_palette: Palette {
        //palette: https://coolors.co/010400-30332e-fffbfc-62bbc1-ec058e
        accent: "#EC058E"
        alternateBase: "#010400"
        base: "#30332E"
        brightText: "white"
        button: "#30332E"
        buttonText: "#FFFBFC"
        dark: "black"
        highlight: "#62BBC1"
        mid: Qt.tint("#30332E", "#010400")
        placeholderText: Qt.lighter("#FFFBFC", 0.6)
        shadow: "black"
        text: "#FFFBFC"
        window: "#30332E"
    }

    readonly property Palette light_palette: Palette {
        accent: "#EC058E"
        alternateBase: "black"
        base: "#30332E"
        brightText: "white"
        button: "#30332E"
        buttonText: "#FFFBFC"
        dark: "black"
        highlight: "#62BBC1"
        shadow: "black"
        text: "#FFFBFC"
        window: "#30332E"
    }

    palette: dark_palette
    color: palette.active.window


    Component.onCompleted: {
    }

    Component.onDestruction: {
        console.info("Saving Configurations for Root Window ...")
        windowsSettings.setValue("rootX", rootWindow.x)
        windowsSettings.setValue("rootY", rootWindow.y)
        windowsSettings.setValue("rootWidth", rootWindow.width)
        windowsSettings.setValue("rootHeight", rootWindow.height)
    }

    menuBar: DMenuBar {
        id: menu_bar

        font: Constants.FONT_SMALL
    }

    header: DHeaderBar {
        id: header_bar
        title: qsTr("DocSpeak")

        text_font: Constants.FONT_SMALL
        page_font: Constants.FONT_SMALL_BOLD
        text_color: palette.active.text
        img_width: Constants.IMAGE_SIZE_SMALL

    }

    footer: DFootBar {
        id: sidebar

        onPageChanged: (index) => {main_view.setIndex(index)}
    }


    DMainView {
        id: main_view

        anchors {
            fill: parent

            margins: 2
            rightMargin: -2*padding
            bottomMargin: -2*padding
        }

        onPageLoaded: (page_name) => {header_bar.setPageName(page_name)}
    }


    Settings {
        id: windowsSettings
        category: "windows"
    }

}
