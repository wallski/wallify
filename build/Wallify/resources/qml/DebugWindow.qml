import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Window
import QtQuick.Layouts

Window {
    id: debugWindow
    width: 700
    height: 500
    color: "#0f0f13"
    title: "Wallify Debug Logs"
    
    // The window is only visible if the setting is true
    visible: appSettings.showDebugWindow
    
    // If user closes window with the X, untoggle the setting automatically
    onClosing: {
        appSettings.showDebugWindow = false
    }
    
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 15
        
        Text {
            text: "Migration Output Logs"
            color: "#e2e2e2"
            font.family: "Roboto"
            font.pixelSize: 18
            font.bold: true
        }
        
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#1e1e28"
            radius: 8
            border.color: "#2a2a35"
            border.width: 1
            
            ScrollView {
                id: scrollView
                anchors.fill: parent
                anchors.margins: 10
                clip: true
                
                TextArea {
                    id: logArea
                    readOnly: true
                    color: "#8be9fd"
                    font.family: "Consolas"
                    font.pixelSize: 12
                    wrapMode: TextEdit.Wrap
                    textFormat: TextEdit.PlainText
                    background: null
                }
            }
        }
    }
    
    Connections {
        target: spotifyMigrator
        function onLogMessage(message) {
            logArea.append(message)
            // Auto scroll to bottom
            scrollView.ScrollBar.vertical.position = 1.0 - scrollView.ScrollBar.vertical.size
        }
    }
}
