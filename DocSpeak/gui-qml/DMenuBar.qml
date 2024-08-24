import QtQuick
import QtQuick.Controls

MenuBar {
    Menu {
        title: qsTr("&File")
        Action { text: qsTr("&New...") }
        Action { text: qsTr("&Open...") }
        Action { text: qsTr("&Save") }
        Action { text: qsTr("Save &As...") }
        MenuSeparator { }
        Action { text: qsTr("&Quit") }
    }
    Menu {
        title: qsTr("&Edit")
        Action { text: qsTr("Cu&t") }
        Action { text: qsTr("&Copy") }
        Action { text: qsTr("&Paste") }
    }
    Menu {
        title: qsTr("&Setting")
        Action { text: qsTr("&View") }
        Action { text: qsTr("&Input/Output") }
        Action { text: qsTr("&Logins") }
        Action { text: qsTr("&Database") }
    }
    Menu {
        title: qsTr("&Help")
        Action { text: qsTr("&About") }
    }
}
