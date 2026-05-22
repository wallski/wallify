import QtQuick
import QtQuick.Window
import QtQuick.Layouts

Rectangle {
    id: titleBar
    height: 32
    color: "transparent"
    
    property Window window: null

    TapHandler {
        onTapped: if (tapCount === 2) toggleMaximized()
        gesturePolicy: TapHandler.DragThreshold
    }

    DragHandler {
        target: null
        onActiveChanged: if (active) window.startSystemMove()
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        Item {
            width: 16
            height: parent.height
        }

        Text {
            text: "Wallify"
            color: "#89899f"
            font.family: "Roboto"
            font.pixelSize: 12
            Layout.alignment: Qt.AlignVCenter
        }

        Item {
            Layout.fillWidth: true
            height: parent.height
        }

        WindowControlButton {
            iconText: "\uE921" 
            onClicked: window.showMinimized()
        }

        WindowControlButton {
            iconText: window.visibility === Window.Maximized ? "\uE923" : "\uE922" 
            onClicked: toggleMaximized()
        }

        WindowControlButton {
            iconText: "\uE8BB" 
            hoverColor: "#ff5f56"
            iconHoverColor: "white"
            onClicked: Qt.quit()
        }
    }

    function toggleMaximized() {
        if (window.visibility === Window.Maximized) {
            window.showNormal()
        } else {
            window.showMaximized()
        }
    }
}
