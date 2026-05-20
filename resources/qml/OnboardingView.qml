import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtQuick.Dialogs

Item {
    id: onboardingView
    
    signal finished()
    
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
        spacing: 40
        
        Text {
            text: "WHERE SHOULD WE SAVE YOUR MUSIC?"
            color: "#e2e2e2"
            font.pixelSize: 42
            font.family: "Bebas Neue"
            font.letterSpacing: 2
            Layout.alignment: Qt.AlignHCenter
        }
        
        Text {
            text: "Pick a folder. We've set a default for you, but you can change it."
            color: "#89899f"
            font.pixelSize: 16
            font.family: "Roboto"
            Layout.alignment: Qt.AlignHCenter
        }
        
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 15
            
            Rectangle {
                Layout.preferredWidth: 400
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
                color: "#5e5e6e"
                radius: 8
                
                Text {
                    anchors.centerIn: parent
                    text: "BROWSE"
                    color: "#e2e2e2"
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
        
        Rectangle {
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 20
            width: 200
            height: 50
            color: "#bd93f9"
            radius: 8
            
            Text {
                anchors.centerIn: parent
                text: "ALL SET, LET'S GO!"
                color: "#18181e"
                font.family: "Roboto"
                font.bold: true
                font.pixelSize: 14
                font.letterSpacing: 1
            }
            
            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    appSettings.setIsFirstRun(false)
                    onboardingView.finished()
                }
            }
        }
    }
}
