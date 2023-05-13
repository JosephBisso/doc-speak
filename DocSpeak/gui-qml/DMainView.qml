import QtQuick 2.15
import QtQuick.Controls 2.15

import "qrc:/Constants.js" as Constants

Frame {
    id: frame

    signal pageLoaded(string name)

    padding: Constants.FRAME_PADDING
    bottomPadding: 3*padding
    rightPadding: 3*padding

    anchors {
        right: parent.right
        bottom: parent.bottom

        rightMargin: -2*padding
        bottomMargin: -2*padding
    }


    background: Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: Constants.FOREGROUND_COLOR }
            GradientStop { position: 1.0; color: Constants.BACKGROUND_COLOR }
        }
        radius: frame.padding
    }

    SwipeView {
        id: swipe_view

        anchors.fill: parent
        orientation: Qt.Vertical
        interactive: false
        clip: true

        Loader {
            id: dashboard_loader
            active: SwipeView.isCurrentItem
            visible: status == Loader.Ready
            asynchronous: true
            onLoaded: {frame.pageLoaded("Dashboard")}
            sourceComponent: Component {
                
                Rectangle {}
            }
        }
        Loader {
            id: doctors_loader
            active: SwipeView.isCurrentItem
            visible: status == Loader.Ready
            asynchronous: true
            onLoaded: {frame.pageLoaded("Doctor Book")}
            sourceComponent: Component {
                DPersonBook {
                    id: doctor_book
                }
            }
        }
        Loader {
            id: patients_loader
            active: SwipeView.isCurrentItem
            visible: status == Loader.Ready
            asynchronous: true
            onLoaded: {frame.pageLoaded("Patient Book")}
            sourceComponent: Component {
                DPersonBook {
                    id: patient_book
                }
            }
        }
        Loader {
            id: calendar_loader
            active: SwipeView.isCurrentItem
            visible: status == Loader.Ready
            asynchronous: true
            onLoaded: {frame.pageLoaded("Calendar")}
            sourceComponent: Component {
                DCalendar {
                    id: calendar
                }
            }
        }
        Loader {
            id: report_loader
            active: SwipeView.isCurrentItem
            visible: status == Loader.Ready
            asynchronous: true
            onLoaded: {frame.pageLoaded("Record Book")}
            sourceComponent: Component {
                DRecordBook{
                    id: record_book
                }
            }
        }
        Loader {
            id: settings_loader
            active: SwipeView.isCurrentItem
            visible: status == Loader.Ready
            asynchronous: true
            onLoaded: {frame.pageLoaded("Settings")}
            sourceComponent: Component {
                Rectangle {}
            }
        }
        Loader {
            id: help_loader
            active: SwipeView.isCurrentItem
            visible: status == Loader.Ready
            asynchronous: true
            onLoaded: {frame.pageLoaded("Help")}
            sourceComponent: Component {
                Rectangle {}
            }
        }
        Loader {
            id: log_loader
            active: SwipeView.isCurrentItem
            visible: status == Loader.Ready
            asynchronous: true
            onLoaded: {frame.pageLoaded("Log In/Out")}
            sourceComponent: Component {
                
                Rectangle {}
            }
        }

    }

    function setIndex(index) {
        swipe_view.setCurrentIndex(index)
    }
    
}