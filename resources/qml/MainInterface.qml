import QtQuick
import QtQuick.Layouts

Item {
    id: mainInterface

    property bool ffmpegOverwriteDialogVisible: false

    Connections {
        target: spotifyMigrator
        function onFfmpegOverwriteRequested() {
            ffmpegOverwriteDialogVisible = true
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 0

            Sidebar {
                Layout.preferredWidth: 250
                Layout.fillHeight: true
                currentIndex: contentStack.currentIndex
                onShowLibrary: contentStack.currentIndex = 0
                onShowMigration: contentStack.currentIndex = 1
                onShowSettings: contentStack.currentIndex = 2
            }

            StackLayout {
                id: contentStack
                Layout.fillWidth: true
                Layout.fillHeight: true
                currentIndex: 0

                LibraryView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }

                MigrationView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }

                SettingsView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }
        }

        BottomPlayerBar {
            Layout.fillWidth: true
        }
    }

    Rectangle {
        id: overlayBackground
        anchors.fill: parent
        color: "#d00a0a0f"
        visible: ffmpegOverwriteDialogVisible
        z: 99999

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            acceptedButtons: Qt.LeftButton | Qt.RightButton
        }

        Rectangle {
            width: 450
            height: 240
            color: "#1e1e28"
            radius: 12
            border.color: "#bd93f9"
            border.width: 1.5
            anchors.centerIn: parent

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 25
                spacing: 15

                RowLayout {
                    spacing: 10
                    Layout.fillWidth: true

                    Rectangle {
                        width: 24
                        height: 24
                        color: "#ff5555"
                        radius: 12

                        Text {
                            anchors.centerIn: parent
                            text: "!"
                            color: "#18181e"
                            font.bold: true
                            font.pixelSize: 14
                        }
                    }

                    Text {
                        text: "OVERWRITE FFMPEG?"
                        color: "#ff5555"
                        font.family: "Bebas Neue"
                        font.pixelSize: 20
                        font.letterSpacing: 1.5
                        Layout.fillWidth: true
                    }
                }

                Text {
                    text: "spotDL detected that FFmpeg is already installed.
It might be older or corrupted. Would you like to overwrite it with a fresh installation?"
                    color: "#e2e2e2"
                    font.family: "Roboto"
                    font.pixelSize: 14
                    lineHeight: 1.2
                    wrapMode: Text.Wrap
                    Layout.fillWidth: true
                }

                Item {
                    Layout.fillHeight: true
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 15

                    Rectangle {
                        Layout.fillWidth: true
                        height: 40
                        color: "#2a2a35"
                        radius: 8
                        border.color: noMouse.containsMouse ? "#89899f" : "transparent"
                        border.width: 1

                        Text {
                            anchors.centerIn: parent
                            text: "NO, KEEP CURRENT"
                            color: "#e2e2e2"
                            font.family: "Roboto"
                            font.bold: true
                            font.pixelSize: 12
                            font.letterSpacing: 1
                        }

                        MouseArea {
                            id: noMouse
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                spotifyMigrator.answerFfmpegOverwrite(false)
                                ffmpegOverwriteDialogVisible = false
                            }
                        }
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        height: 40
                        color: "#bd93f9"
                        radius: 8
                        border.color: yesMouse.containsMouse ? "#ffffff" : "transparent"
                        border.width: 1

                        Text {
                            anchors.centerIn: parent
                            text: "YES, OVERWRITE"
                            color: "#18181e"
                            font.family: "Roboto"
                            font.bold: true
                            font.pixelSize: 12
                            font.letterSpacing: 1
                        }

                        MouseArea {
                            id: yesMouse
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                spotifyMigrator.answerFfmpegOverwrite(true)
                                ffmpegOverwriteDialogVisible = false
                            }
                        }
                    }
                }
            }
        }
    }
}
