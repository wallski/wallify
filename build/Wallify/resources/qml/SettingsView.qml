import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtQuick.Dialogs

Item {
    id: settingsView

    FolderDialog {
        id: folderDialog
        title: "Choose Library Folder"
        currentFolder: appSettings.libraryPath
        onAccepted: {
            appSettings.libraryPath = selectedFolder
        }
    }

    ColumnLayout {
        anchors.centerIn: parent
        width: parent.width * 0.6
        spacing: 30

        Text {
            text: "SETTINGS"
            color: "#e2e2e2"
            font.pixelSize: 42
            font.family: "Bebas Neue"
            font.letterSpacing: 2
            Layout.alignment: Qt.AlignHCenter
        }

        Rectangle {
            Layout.fillWidth: true
            height: 80
            color: "#1e1e28"
            radius: 8
            border.color: "#2a2a35"
            border.width: 1

            RowLayout {
                anchors.fill: parent
                anchors.margins: 20

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 4

                    Text {
                        text: "Discord Rich Presence"
                        color: "#e2e2e2"
                        font.family: "Roboto"
                        font.pixelSize: 16
                        font.bold: true
                    }

                    Text {
                        text: "Show what you're listening to on your Discord profile."
                        color: "#89899f"
                        font.family: "Roboto"
                        font.pixelSize: 13
                    }
                }

                Switch {
                    checked: true
                    onCheckedChanged: {
                        if (checked) {
                            discordRPC.initialize()
                        } else {
                            discordRPC.shutdown()
                        }
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 80
            color: "#1e1e28"
            radius: 8
            border.color: "#2a2a35"
            border.width: 1

            RowLayout {
                anchors.fill: parent
                anchors.margins: 20

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 4

                    Text {
                        text: "Show Debug Console"
                        color: "#e2e2e2"
                        font.family: "Roboto"
                        font.pixelSize: 16
                        font.bold: true
                    }

                    Text {
                        text: "Opens a CMD window to see backend logs during migration."
                        color: "#89899f"
                        font.family: "Roboto"
                        font.pixelSize: 13
                    }
                }

                Switch {
                    checked: appSettings.showDebugWindow
                    onCheckedChanged: appSettings.showDebugWindow = checked
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 100
            color: "#1e1e28"
            radius: 8
            border.color: "#2a2a35"
            border.width: 1

            ColumnLayout {
                anchors.centerIn: parent
                spacing: 10

                Text {
                    text: "Having issues? This will reset Wallify setup (does not delete your music)."
                    color: "#89899f"
                    font.family: "Roboto"
                    font.pixelSize: 13
                    Layout.alignment: Qt.AlignHCenter
                }

                Rectangle {
                    Layout.alignment: Qt.AlignHCenter
                    width: 200
                    height: 40
                    color: "#ff5555"
                    radius: 8

                    Text {
                        anchors.centerIn: parent
                        text: "RESET TO ONBOARDING"
                        color: "#18181e"
                        font.family: "Roboto"
                        font.bold: true
                        font.pixelSize: 14
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            appSettings.factoryReset()
                        }
                    }
                }
            }
        }

        Item {
            Layout.fillHeight: true
        }

        ColumnLayout {
            Layout.fillWidth: true
            spacing: 10

            Text {
                text: "Local Music Library Location"
                color: "#e2e2e2"
                font.family: "Roboto"
                font.pixelSize: 16
                font.bold: true
            }

            Text {
                text: "All migrated songs and playlists will be saved here."
                color: "#89899f"
                font.family: "Roboto"
                font.pixelSize: 14
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 15

                Rectangle {
                    Layout.fillWidth: true
                    height: 48
                    color: "#1e1e28"
                    radius: 8
                    border.color: "#2a2a35"
                    border.width: 1

                    Text {
                        anchors.fill: parent
                        anchors.margins: 10
                        verticalAlignment: Text.AlignVCenter
                        text: appSettings.libraryPath
                        color: "#89899f"
                        font.family: "Roboto"
                        font.pixelSize: 14
                        elide: Text.ElideRight
                    }
                }

                Rectangle {
                    width: 120
                    height: 48
                    color: "#bd93f9"
                    radius: 8

                    Text {
                        anchors.centerIn: parent
                        text: "CHANGE"
                        color: "#18181e"
                        font.family: "Roboto"
                        font.bold: true
                        font.pixelSize: 14
                        font.letterSpacing: 1
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: folderDialog.open()
                    }
                }
            }
        }
    }
}
