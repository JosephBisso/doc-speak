import QtQuick
import QtQuick.Controls

import "Constants.js" as Constants

Page {
    id: swipe_view

    signal pageLoaded(string name)

    padding: Constants.FRAME_PADDING
    bottomPadding: 3*padding
    rightPadding: 3*padding

    anchors.fill: parent
    clip: true




    Loader {
        id: dashboard_loader
        active: SwipeView.isCurrentItem
        visible: status == Loader.Ready
        asynchronous: true
        onLoaded: {swipe_view.pageLoaded("Dashboard")}
        sourceComponent: Component {

            Frame {}
        }
    }
    Loader {
        id: doctors_loader
        active: SwipeView.isCurrentItem
        visible: status == Loader.Ready
        asynchronous: true
        onLoaded: {swipe_view.pageLoaded("Doctor Book")}
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
        onLoaded: {swipe_view.pageLoaded("Patient Book")}
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
        onLoaded: {swipe_view.pageLoaded("Calendar")}
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
        onLoaded: {swipe_view.pageLoaded("Record Book")}
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
        onLoaded: {swipe_view.pageLoaded("Settings")}
        sourceComponent: Component {
            Frame {}
        }
    }
    Loader {
        id: help_loader
        active: SwipeView.isCurrentItem
        visible: status == Loader.Ready
        asynchronous: true
        onLoaded: {swipe_view.pageLoaded("Help")}
        sourceComponent: Component {
            Frame {}
        }
    }
    Loader {
        id: log_loader
        active: SwipeView.isCurrentItem
        visible: status == Loader.Ready
        asynchronous: true
        onLoaded: {swipe_view.pageLoaded("Log In/Out")}
        sourceComponent: Component {

            Frame {}
        }
    }

    function setIndex(index: int) : void{
        swipe_view.setCurrentIndex(index)
    }

}


