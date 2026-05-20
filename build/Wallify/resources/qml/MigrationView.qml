import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

Item {
    id: migrationView
    
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
