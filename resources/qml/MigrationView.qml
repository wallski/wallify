import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

Item {
    id: migrationView

    property int currentTab: 0

    Connections {
        target: spotifyMigrator
        function onFfmpegOverwriteRequested() {
            ffmpegPopup.open()
        }
    }

    Popup {
        id: ffmpegPopup
        anchors.centerIn: parent
        width: 400
        height: 200
        modal: true
        focus: true
        closePolicy: Popup.NoAutoClose

        background: Rectangle {
            color: "#1e1e28"
            radius: 8
            border.color: "#2a2a35"
            border.width: 1
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 15

            Text {
                text: "FFmpeg is already installed"
                color: "#e2e2e2"
                font.family: "Roboto"
                font.pixelSize: 18
                font.bold: true
                Layout.alignment: Qt.AlignHCenter
            }

            Text {
                text: "Do you want to overwrite it with a fresh download? (Recommended if you are having issues)"
                color: "#89899f"
                font.family: "Roboto"
                font.pixelSize: 14
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter
                horizontalAlignment: Text.AlignHCenter
            }

            Item { Layout.fillHeight: true }

            RowLayout {
                Layout.fillWidth: true
                spacing: 15

                Rectangle {
                    Layout.fillWidth: true
                    height: 40
                    color: "#2a2a35"
                    radius: 8

                    Text {
                        anchors.centerIn: parent
                        text: "NO (Keep Existing)"
                        color: "#e2e2e2"
                        font.family: "Roboto"
                        font.bold: true
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            spotifyMigrator.answerFfmpegOverwrite(false)
                            ffmpegPopup.close()
                        }
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    height: 40
                    color: "#bd93f9"
                    radius: 8

                    Text {
                        anchors.centerIn: parent
                        text: "YES (Overwrite)"
                        color: "#18181e"
                        font.family: "Roboto"
                        font.bold: true
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            spotifyMigrator.answerFfmpegOverwrite(true)
                            ffmpegPopup.close()
                        }
                    }
                }
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 30
        spacing: 20

        Text {
            text: "MIGRATION"
            color: "#e2e2e2"
            font.pixelSize: 42
            font.family: "Bebas Neue"
            font.letterSpacing: 2
        }

        Text {
            text: "Download songs from your favorite platforms."
            color: "#89899f"
            font.pixelSize: 16
            font.family: "Roboto"
        }

        // Tab Bar
        RowLayout {
            spacing: 0

            Rectangle {
                width: 140
                height: 40
                color: migrationView.currentTab === 0 ? "#bd93f9" : "transparent"
                radius: 8
                border.color: migrationView.currentTab === 0 ? "#bd93f9" : "#2a2a35"
                border.width: 1

                Text {
                    anchors.centerIn: parent
                    text: "Spotify"
                    color: migrationView.currentTab === 0 ? "#18181e" : "#e2e2e2"
                    font.family: "Roboto"
                    font.bold: true
                    font.pixelSize: 13
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: migrationView.currentTab = 0
                }
            }

            Rectangle {
                width: 140
                height: 40
                color: migrationView.currentTab === 1 ? "#bd93f9" : "transparent"
                radius: 8
                border.color: migrationView.currentTab === 1 ? "#bd93f9" : "#2a2a35"
                border.width: 1

                Text {
                    anchors.centerIn: parent
                    text: "YouTube"
                    color: migrationView.currentTab === 1 ? "#18181e" : "#e2e2e2"
                    font.family: "Roboto"
                    font.bold: true
                    font.pixelSize: 13
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: migrationView.currentTab = 1
                }
            }

            Rectangle {
                width: 140
                height: 40
                color: migrationView.currentTab === 2 ? "#bd93f9" : "transparent"
                radius: 8
                border.color: migrationView.currentTab === 2 ? "#bd93f9" : "#2a2a35"
                border.width: 1

                Text {
                    anchors.centerIn: parent
                    text: "SoundCloud"
                    color: migrationView.currentTab === 2 ? "#18181e" : "#e2e2e2"
                    font.family: "Roboto"
                    font.bold: true
                    font.pixelSize: 13
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: migrationView.currentTab = 2
                }
            }
        }

        // Spotify Tab
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: migrationView.currentTab === 0
            spacing: 20

            Text {
                text: "SPOTIFY MIGRATION"
                color: "#e2e2e2"
                font.pixelSize: 28
                font.family: "Bebas Neue"
                font.letterSpacing: 2
            }

            Text {
                text: "Paste a Spotify track, album, or playlist URL to download."
                color: "#89899f"
                font.pixelSize: 14
                font.family: "Roboto"
            }

            Rectangle {
                Layout.fillWidth: true
                height: 48
                color: "#1e1e28"
                radius: 8
                border.color: "#2a2a35"
                border.width: 1

                TextInput {
                    id: spotifyUrlInput
                    anchors.fill: parent
                    anchors.margins: 10
                    verticalAlignment: TextInput.AlignVCenter
                    color: "#e2e2e2"
                    font.family: "Roboto"
                    font.pixelSize: 14
                    selectByMouse: true
                    clip: true
                    Text {
                        text: "Paste Spotify URL..."
                        color: "#5e5e6e"
                        visible: !spotifyUrlInput.text
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        font.family: "Roboto"
                        font.pixelSize: 14
                    }
                }
            }

            Rectangle {
                Layout.alignment: Qt.AlignHCenter
                width: 180
                height: 44
                color: spotifyMigrator.isWorking ? "#5e5e6e" : "#bd93f9"
                radius: 8

                Text {
                    anchors.centerIn: parent
                    text: spotifyMigrator.isWorking ? "MIGRATING..." : "MIGRATE NOW"
                    color: "#18181e"
                    font.family: "Roboto"
                    font.bold: true
                    font.pixelSize: 14
                    font.letterSpacing: 1
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: !spotifyMigrator.isWorking
                    cursorShape: spotifyMigrator.isWorking ? Qt.ArrowCursor : Qt.PointingHandCursor
                    onClicked: {
                        if (!spotifyMigrator.isWorking && spotifyUrlInput.text.length > 0) {
                            spotifyMigrator.startMigration(spotifyUrlInput.text)
                        }
                    }
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                visible: spotifyMigrator.isWorking || spotifyMigrator.progress > 0
                spacing: 10

                Text {
                    text: spotifyMigrator.statusText
                    color: "#e2e2e2"
                    font.family: "Roboto"
                    font.pixelSize: 14
                }

                Rectangle {
                    Layout.fillWidth: true
                    height: 6
                    color: "#1e1e28"
                    radius: 3

                    Rectangle {
                        width: parent.width * (spotifyMigrator.progress / 100.0)
                        height: parent.height
                        color: "#bd93f9"
                        radius: 3

                        Behavior on width {
                            NumberAnimation { duration: 250 }
                        }
                    }
                }
            }
        }

        // YouTube Tab
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: migrationView.currentTab === 1
            spacing: 20

            Text {
                text: "YOUTUBE DOWNLOAD"
                color: "#e2e2e2"
                font.pixelSize: 28
                font.family: "Bebas Neue"
                font.letterSpacing: 2
            }

            Text {
                text: "Paste a YouTube video or playlist URL to download as MP3."
                color: "#89899f"
                font.pixelSize: 14
                font.family: "Roboto"
            }

            Rectangle {
                Layout.fillWidth: true
                height: 48
                color: "#1e1e28"
                radius: 8
                border.color: "#2a2a35"
                border.width: 1

                TextInput {
                    id: youtubeUrlInput
                    anchors.fill: parent
                    anchors.margins: 10
                    verticalAlignment: TextInput.AlignVCenter
                    color: "#e2e2e2"
                    font.family: "Roboto"
                    font.pixelSize: 14
                    selectByMouse: true
                    clip: true
                    Text {
                        text: "Paste YouTube URL..."
                        color: "#5e5e6e"
                        visible: !youtubeUrlInput.text
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        font.family: "Roboto"
                        font.pixelSize: 14
                    }
                }
            }

            Rectangle {
                Layout.alignment: Qt.AlignHCenter
                width: 180
                height: 44
                color: youtubeMigrator.isWorking ? "#5e5e6e" : "#ff0000"
                radius: 8

                Text {
                    anchors.centerIn: parent
                    text: youtubeMigrator.isWorking ? "DOWNLOADING..." : "DOWNLOAD"
                    color: "#ffffff"
                    font.family: "Roboto"
                    font.bold: true
                    font.pixelSize: 14
                    font.letterSpacing: 1
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: !youtubeMigrator.isWorking
                    cursorShape: youtubeMigrator.isWorking ? Qt.ArrowCursor : Qt.PointingHandCursor
                    onClicked: {
                        if (!youtubeMigrator.isWorking && youtubeUrlInput.text.length > 0) {
                            youtubeMigrator.startMigration(youtubeUrlInput.text)
                        }
                    }
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                visible: youtubeMigrator.isWorking || youtubeMigrator.progress > 0
                spacing: 10

                Text {
                    text: youtubeMigrator.statusText
                    color: "#e2e2e2"
                    font.family: "Roboto"
                    font.pixelSize: 14
                }

                Rectangle {
                    Layout.fillWidth: true
                    height: 6
                    color: "#1e1e28"
                    radius: 3

                    Rectangle {
                        width: parent.width * (youtubeMigrator.progress / 100.0)
                        height: parent.height
                        color: "#ff0000"
                        radius: 3

                        Behavior on width {
                            NumberAnimation { duration: 250 }
                        }
                    }
                }
            }
        }

        // SoundCloud Tab
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: migrationView.currentTab === 2
            spacing: 20

            Text {
                text: "SOUNDCLOUD DOWNLOAD"
                color: "#e2e2e2"
                font.pixelSize: 28
                font.family: "Bebas Neue"
                font.letterSpacing: 2
            }

            Text {
                text: "Paste a SoundCloud track or playlist URL to download."
                color: "#89899f"
                font.pixelSize: 14
                font.family: "Roboto"
            }

            Rectangle {
                Layout.fillWidth: true
                height: 48
                color: "#1e1e28"
                radius: 8
                border.color: "#2a2a35"
                border.width: 1

                TextInput {
                    id: soundcloudUrlInput
                    anchors.fill: parent
                    anchors.margins: 10
                    verticalAlignment: TextInput.AlignVCenter
                    color: "#e2e2e2"
                    font.family: "Roboto"
                    font.pixelSize: 14
                    selectByMouse: true
                    clip: true
                    Text {
                        text: "Paste SoundCloud URL..."
                        color: "#5e5e6e"
                        visible: !soundcloudUrlInput.text
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        font.family: "Roboto"
                        font.pixelSize: 14
                    }
                }
            }

            Rectangle {
                Layout.alignment: Qt.AlignHCenter
                width: 180
                height: 44
                color: soundcloudMigrator.isWorking ? "#5e5e6e" : "#ff5500"
                radius: 8

                Text {
                    anchors.centerIn: parent
                    text: soundcloudMigrator.isWorking ? "DOWNLOADING..." : "DOWNLOAD"
                    color: "#ffffff"
                    font.family: "Roboto"
                    font.bold: true
                    font.pixelSize: 14
                    font.letterSpacing: 1
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: !soundcloudMigrator.isWorking
                    cursorShape: soundcloudMigrator.isWorking ? Qt.ArrowCursor : Qt.PointingHandCursor
                    onClicked: {
                        if (!soundcloudMigrator.isWorking && soundcloudUrlInput.text.length > 0) {
                            soundcloudMigrator.startMigration(soundcloudUrlInput.text)
                        }
                    }
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                visible: soundcloudMigrator.isWorking || soundcloudMigrator.progress > 0
                spacing: 10

                Text {
                    text: soundcloudMigrator.statusText
                    color: "#e2e2e2"
                    font.family: "Roboto"
                    font.pixelSize: 14
                }

                Rectangle {
                    Layout.fillWidth: true
                    height: 6
                    color: "#1e1e28"
                    radius: 3

                    Rectangle {
                        width: parent.width * (soundcloudMigrator.progress / 100.0)
                        height: parent.height
                        color: "#ff5500"
                        radius: 3

                        Behavior on width {
                            NumberAnimation { duration: 250 }
                        }
                    }
                }
            }
        }

        Item { Layout.fillHeight: true }
    }
}
