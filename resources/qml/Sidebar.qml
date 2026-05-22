import QtQuick
import QtQuick.Layouts

Rectangle {
    id: sidebar
    color: "#141419"

    property int currentIndex: 1

    signal showLibrary()
    signal showMigration()
    signal showSettings()

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        Text {
            text: "Your Library"
            color: "#89899f"
            font.family: "Roboto"
            font.bold: true
        }

        Rectangle {
            Layout.fillWidth: true
            height: 40
            color: "transparent"
            radius: 6

            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
                text: "Library"
                color: sidebar.currentIndex === 0 ? "#bd93f9" : "#e2e2e2"
                font.family: "Roboto"
                font.pixelSize: 14
                font.bold: sidebar.currentIndex === 0
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                hoverEnabled: true
                onEntered: parent.color = "#1e1e28"
                onExited: parent.color = "transparent"
                onClicked: sidebar.showLibrary()
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 40
            color: "transparent"
            radius: 6

            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
                text: "Migration"
                color: sidebar.currentIndex === 1 ? "#bd93f9" : "#e2e2e2"
                font.family: "Roboto"
                font.pixelSize: 14
                font.bold: sidebar.currentIndex === 1
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                hoverEnabled: true
                onEntered: parent.color = "#1e1e28"
                onExited: parent.color = "transparent"
                onClicked: sidebar.showMigration()
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 40
            color: "transparent"
            radius: 6

            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
                text: "Settings"
                color: sidebar.currentIndex === 2 ? "#bd93f9" : "#e2e2e2"
                font.family: "Roboto"
                font.pixelSize: 14
                font.bold: sidebar.currentIndex === 2
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                hoverEnabled: true
                onEntered: parent.color = "#1e1e28"
                onExited: parent.color = "transparent"
                onClicked: sidebar.showSettings()
            }
        }

        Item { Layout.fillHeight: true } 
    }
}
