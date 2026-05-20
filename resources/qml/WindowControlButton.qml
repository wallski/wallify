import QtQuick
import QtQuick.Controls.Basic

Rectangle {
    id: controlButton
    width: 46
    height: 32
    color: "transparent"

    property string iconText: ""
    property color hoverColor: "#2a2a35"
    property color iconColor: "#89899f"
    property color iconHoverColor: "#e2e2e2"
    
    signal clicked()

    Text {
        anchors.centerIn: parent
        text: controlButton.iconText
        color: mouseArea.containsMouse ? controlButton.iconHoverColor : controlButton.iconColor
        font.family: "Segoe MDL2 Assets" 
        font.pixelSize: 10
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: controlButton.clicked()
    }

    states: [
        State {
            name: "hovered"
            when: mouseArea.containsMouse
            PropertyChanges {
                target: controlButton
                color: controlButton.hoverColor
            }
        }
    ]

    transitions: [
        Transition {
            ColorAnimation { duration: 100 }
        }
    ]
}
