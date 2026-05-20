import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

Item {
    id: migrationView
    
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
        anchors.centerIn: parent
        width: parent.width * 0.6
        spacing: 20
        
        Text {
            text: "SPOTIFY MIGRATION"
            color: "#e2e2e2"
            font.pixelSize: 42
            font.family: "Bebas Neue"
            font.letterSpacing: 2
            Layout.alignment: Qt.AlignHCenter
        }
        
        Text {
            text: "Download songs and playlists seamlessly."
            color: "#89899f"
            font.pixelSize: 16
            font.family: "Roboto"
            Layout.alignment: Qt.AlignHCenter
        }
        
        Rectangle {
            Layout.fillWidth: true
            height: 48
            color: "#1e1e28"
            radius: 8
            border.color: "#2a2a35"
            border.width: 1
            
            TextInput {
                id: urlInput
                anchors.fill: parent
                anchors.margins: 10
                verticalAlignment: TextInput.AlignVCenter
                color: "#e2e2e2"
                font.family: "Roboto"
                font.pixelSize: 14
                selectByMouse: true
                clip: true
                Text {
                    text: "Paste Spotify Track/Playlist URL..."
                    color: "#5e5e6e"
                    visible: !urlInput.text
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    font.family: "Roboto"
                    font.pixelSize: 14
                }
            }
        }
        
        Rectangle {
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 10
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
                    if (!spotifyMigrator.isWorking && urlInput.text.length > 0) {
                        spotifyMigrator.startMigration(urlInput.text)
                    }
                }
            }
        }
        
        ColumnLayout {
            Layout.fillWidth: true
            Layout.topMargin: 20
            visible: spotifyMigrator.isWorking || spotifyMigrator.progress > 0
            spacing: 10
            
            Text {
                text: spotifyMigrator.statusText
                color: "#e2e2e2"
                font.family: "Roboto"
                font.pixelSize: 14
                Layout.alignment: Qt.AlignHCenter
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
}
