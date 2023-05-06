import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Frame {
    id: frame

    property int img_size: 30

    padding: 5
    width: 70

    anchors {
        left: parent.left
        bottom: parent.bottom
    }

    background: Rectangle {
        anchors.fill: parent
        color: "transparent"
    }

    ColumnLayout {
        anchors.fill: parent
        
        Repeater {
            model: [
                "Dashboard",
                "Doctors",
                "Patients",
                "Calendar",
                "Report",
                "Settings",
                "Help"
            ]

            delegate: SideElement  {
                display: AbstractButton.TextUnderIcon
                source : `qrc/../assets/${modelData}.png`
                text: modelData

                onClicked: frame.actionForButton(modelData)
                
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }

        SideElement  {
            id: log_button
            property bool loggedIn: false
            property string msg: loggedIn ? "Log Out" : "Log In"
            property string context: loggedIn ? "LogOut" : "LogIn"

            display: AbstractButton.TextUnderIcon
            source : `qrc/../assets/${log_button.context}.png`
            text: log_button.msg

            onClicked: frame.actionForButton(context)
            
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    function actionForButton(button) {
        console.log(`${button} clicked`)
        switch(button) {
            case "Dashboard":
                break;
            case "Doctors":
                break;
            case "Patients":
                break;
            case "Calendar":
                break;
            case "Report":
                break;
            case "Settings":
                break;
            case "Help":
                break;
            case "LogIn":
                break;
            case "LogOut":
                break;
        }
    }

    component SideElement: CheckDelegate  {
        id: control
        text: qsTr("CheckDelegate")
        property url source: "qrc/../assets/Help.png"

        contentItem: Column {
            id: column
            anchors.centerIn: control
            padding: 5
            Image {
                anchors.horizontalCenter: column.horizontalCenter
                width: frame.img_size; height: width
                fillMode: Image.PreserveAspectFit
                horizontalAlignment: Image.AlignHCenter
                source: control.source
            }
            Text {
                anchors.horizontalCenter: column.horizontalCenter
                text: control.text
                font: control.font
                opacity: enabled ? 1.0 : 0.3
                color: control.down || control.checked  ? "white" : "black"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignHCenter
            }
        }

        indicator: Rectangle {
            implicitWidth: 5
            height: control.height / 2
            x: 0
            y: control.topPadding + control.availableHeight / 2 - height / 2
            radius: 5
            color: control.down || control.checked  ? "blue" : "transparent"
        }

        background: Rectangle {
            implicitWidth: 100
            implicitHeight: 40
            radius: 10
            color: {
                if (control.down) {return "skyblue";}
                else if (control.highlighted) {return "lightblue";}
                else if (control.checked) {return "skyblue";}
                else {return "transparent";}
            }
        }
    }
}