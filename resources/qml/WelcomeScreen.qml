import QtQuick
import QtQuick.Layouts

Item {
    id: welcomeScreen
    
    signal getStarted()

    Rectangle {
        anchors.fill: parent
        anchors.margins: 20
        color: "#18181e"
        radius: 12
        
        border.color: "#2a2a35"
        border.width: 1

        ColumnLayout {
            anchors.centerIn: parent
            spacing: 20

            Text {
                text: "WALLIFY"
                color: "#e2e2e2"
                font.pixelSize: 64
                font.family: "Bebas Neue" 
                font.letterSpacing: 4
                Layout.alignment: Qt.AlignHCenter
            }

            Text {
                text: "Local Spotify Alternative"
                color: "#89899f"
                font.pixelSize: 18
                font.family: "Roboto"
                Layout.alignment: Qt.AlignHCenter
            }
            
            Rectangle {
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 20
                width: 200
                height: 48
                color: "#bd93f9"
                radius: 8

                Text {
                    anchors.centerIn: parent
                    text: "GET STARTED"
                    color: "#18181e"
                    font.family: "Roboto"
                    font.bold: true
                    font.pixelSize: 14
                    font.letterSpacing: 1
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: welcomeScreen.getStarted()
                }
            }
        }
    }
}
