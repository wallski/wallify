import QtQuick
import QtQuick.Layouts

Rectangle {
    id: bottomBar
    color: "#18181e"
    height: 90
    
    Rectangle {
        width: parent.width
        height: 1
        color: "#2a2a35"
        anchors.top: parent.top
    }
    
    RowLayout {
        anchors.fill: parent
        anchors.margins: 15
        spacing: 20
        
        Rectangle {
            width: 60
            height: 60
            radius: 6
            color: "#2a2a35"
        }
        
        ColumnLayout {
            spacing: 2
            
            Text {
                text: "Song Title"
                color: "#e2e2e2"
                font.family: "Roboto"
                font.bold: true
                font.pixelSize: 14
            }
            
            Text {
                text: "Artist Name"
                color: "#89899f"
                font.family: "Roboto"
                font.pixelSize: 12
            }
        }
        
        Item { Layout.fillWidth: true } 
        
        ColumnLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 10
            
            RowLayout {
                spacing: 20
                Layout.alignment: Qt.AlignHCenter
                
                Text {
                    text: "\uE892" 
                    color: "#89899f"
                    font.family: "Segoe MDL2 Assets"
                    font.pixelSize: 16
                }
                
                Rectangle {
                    width: 40
                    height: 40
                    radius: 20
                    color: "#e2e2e2"
                    
                    Text {
                        anchors.centerIn: parent
                        text: "\uE768" 
                        color: "#18181e"
                        font.family: "Segoe MDL2 Assets"
                        font.pixelSize: 16
                    }
                }
                
                Text {
                    text: "\uE893" 
                    color: "#89899f"
                    font.family: "Segoe MDL2 Assets"
                    font.pixelSize: 16
                }
            }
            
            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                spacing: 10
                
                Text {
                    text: "0:00"
                    color: "#89899f"
                    font.family: "Roboto"
                    font.pixelSize: 12
                }
                
                Rectangle {
                    Layout.preferredWidth: 300
                    height: 4
                    color: "#2a2a35"
                    radius: 2
                    
                    Rectangle {
                        width: 50
                        height: parent.height
                        color: "#e2e2e2"
                        radius: 2
                    }
                }
                
                Text {
                    text: "3:45"
                    color: "#89899f"
                    font.family: "Roboto"
                    font.pixelSize: 12
                }
            }
        }
        
        Item { Layout.fillWidth: true } 
    }
}
