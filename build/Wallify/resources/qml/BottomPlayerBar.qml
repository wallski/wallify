import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic

Rectangle {
    id: bottomBar
    color: "#18181e"
    height: 90

    function formatDuration(ms) {
        if (!ms || ms <= 0) return "0:00";
        var totalSecs = Math.floor(ms / 1000);
        var seconds = totalSecs % 60;
        var minutes = Math.floor(totalSecs / 60);
        return minutes + ":" + (seconds < 10 ? "0" : "") + seconds;
    }

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
            Layout.preferredWidth: 60
            Layout.preferredHeight: 60
            radius: 6
            color: "#2a2a35"
            clip: true

            Image {
                anchors.fill: parent
                source: (audioPlayer.currentTrack && audioPlayer.currentTrack.hasCover) ? "image://covers/" + encodeURIComponent(audioPlayer.currentTrack.filePath) : ""
                fillMode: Image.PreserveAspectCrop
                asynchronous: true
                visible: audioPlayer.currentTrack && audioPlayer.currentTrack.hasCover
            }

            Rectangle {
                anchors.fill: parent
                color: "#23232f"
                visible: !(audioPlayer.currentTrack && audioPlayer.currentTrack.hasCover)

                Text {
                    anchors.centerIn: parent
                    text: ""
                    font.family: "Segoe MDL2 Assets"
                    font.pixelSize: 20
                    color: "#4e4e5a"
                }
            }
        }

        ColumnLayout {
            spacing: 2
            Layout.maximumWidth: 200

            Text {
                text: (audioPlayer.currentTrack && audioPlayer.currentTrack.title) ? audioPlayer.currentTrack.title : "Not Playing"
                color: "#e2e2e2"
                font.family: "Roboto"
                font.bold: true
                font.pixelSize: 14
                elide: Text.ElideRight
                Layout.fillWidth: true
            }

            Text {
                text: (audioPlayer.currentTrack && audioPlayer.currentTrack.artist) ? audioPlayer.currentTrack.artist : "Select a track to start"
                color: "#89899f"
                font.family: "Roboto"
                font.pixelSize: 12
                elide: Text.ElideRight
                Layout.fillWidth: true
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
                    text: ""
                    color: audioPlayer.shuffleEnabled ? "#bd93f9" : (shuffleMouse.containsMouse ? "#e2e2e2" : "#89899f")
                    font.family: "Segoe MDL2 Assets"
                    font.pixelSize: 16

                    Rectangle {
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: parent.bottom
                        anchors.topMargin: 2
                        width: 4
                        height: 4
                        radius: 2
                        color: "#bd93f9"
                        visible: audioPlayer.shuffleEnabled
                    }

                    MouseArea {
                        id: shuffleMouse
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            audioPlayer.setShuffleEnabled(!audioPlayer.shuffleEnabled)
                        }
                    }
                }

                Text {
                    text: ""
                    color: prevMouse.containsMouse ? "#bd93f9" : "#89899f"
                    font.family: "Segoe MDL2 Assets"
                    font.pixelSize: 16

                    MouseArea {
                        id: prevMouse
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: audioPlayer.previous()
                    }
                }

                Rectangle {
                    Layout.preferredWidth: 40
                    Layout.preferredHeight: 40
                    radius: 20
                    color: playMouse.containsMouse ? "#9a6fd6" : "#bd93f9"

                    Behavior on color { ColorAnimation { duration: 100 } }

                    Text {
                        anchors.centerIn: parent
                        anchors.horizontalCenterOffset: audioPlayer.isPlaying ? 0 : 3
                        text: audioPlayer.isPlaying ? "" : ""
                        color: "#18181e"
                        font.family: "Segoe MDL2 Assets"
                        font.pixelSize: 14
                        font.bold: true
                    }

                    MouseArea {
                        id: playMouse
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            if (audioPlayer.isPlaying) {
                                audioPlayer.pause()
                            } else {
                                audioPlayer.play()
                            }
                        }
                    }
                }

                Text {
                    text: ""
                    color: nextMouse.containsMouse ? "#bd93f9" : "#89899f"
                    font.family: "Segoe MDL2 Assets"
                    font.pixelSize: 16

                    MouseArea {
                        id: nextMouse
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: audioPlayer.next()
                    }
                }

                Text {
                    id: loopText
                    text: audioPlayer.loopMode === 2 ? "" : ""
                    color: audioPlayer.loopMode > 0 ? "#bd93f9" : (loopMouse.containsMouse ? "#e2e2e2" : "#89899f")
                    font.family: "Segoe MDL2 Assets"
                    font.pixelSize: 16
                    Layout.alignment: Qt.AlignVCenter

                    Rectangle {
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: parent.bottom
                        anchors.topMargin: 2
                        width: 4
                        height: 4
                        radius: 2
                        color: "#bd93f9"
                        visible: audioPlayer.loopMode > 0
                    }

                    MouseArea {
                        id: loopMouse
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            audioPlayer.loopMode = (audioPlayer.loopMode + 1) % 3
                        }
                    }
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                spacing: 10

                Text {
                    text: bottomBar.formatDuration(audioPlayer.position)
                    color: "#89899f"
                    font.family: "Roboto"
                    font.pixelSize: 12
                }

                Slider {
                    id: progressSlider
                    Layout.preferredWidth: 350
                    from: 0
                    to: audioPlayer.duration > 0 ? audioPlayer.duration : 100
                    live: false
                    value: audioPlayer.position
                    onMoved: {
                        audioPlayer.setPosition(value)
                    }
                    onPressedChanged: {
                        if (!pressed) {
                            audioPlayer.setPosition(value)
                        }
                    }

                    Connections {
                        target: audioPlayer
                        function onPositionChanged() {
                            if (!progressSlider.pressed) {
                                progressSlider.value = audioPlayer.position
                            }
                        }
                    }

                    background: Rectangle {
                        x: progressSlider.leftPadding
                        y: progressSlider.topPadding + progressSlider.availableHeight / 2 - height / 2
                        implicitWidth: 200
                        implicitHeight: 4
                        width: progressSlider.availableWidth
                        height: implicitHeight
                        radius: 2
                        color: "#2a2a35"

                        Rectangle {
                            width: progressSlider.visualPosition * parent.width
                            height: parent.height
                            color: "#bd93f9"
                            radius: 2
                        }
                    }

                    handle: Rectangle {
                        x: progressSlider.leftPadding + progressSlider.visualPosition * (progressSlider.availableWidth - width)
                        y: progressSlider.topPadding + progressSlider.availableHeight / 2 - height / 2
                        implicitWidth: 10
                        implicitHeight: 10
                        radius: 5
                        color: "#bd93f9"
                        visible: progressSlider.hovered || progressSlider.pressed
                    }
                }

                Text {
                    text: bottomBar.formatDuration(audioPlayer.duration)
                    color: "#89899f"
                    font.family: "Roboto"
                    font.pixelSize: 12
                }
            }
        }

        Item { Layout.fillWidth: true } 

        RowLayout {
            spacing: 10
            Layout.alignment: Qt.AlignRight

            Item {
                Layout.preferredWidth: 20
                Layout.preferredHeight: 20
                Layout.alignment: Qt.AlignVCenter

                Text {
                    anchors.centerIn: parent
                    text: volumeSlider.value === 0 ? "" : (volumeSlider.value < 0.4 ? "" : (volumeSlider.value < 0.8 ? "" : ""))
                    color: muteMouse.containsMouse ? "#bd93f9" : "#89899f"
                    font.family: "Segoe MDL2 Assets"
                    font.pixelSize: 16
                }

                // Mute overlay (a slash through the icon)
                Text {
                    anchors.centerIn: parent
                    text: "/"
                    color: "#ff5555"
                    font.family: "Roboto"
                    font.pixelSize: 22
                    font.bold: true
                    visible: volumeSlider.value === 0
                }

                MouseArea {
                    id: muteMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    property real previousVolume: 1.0
                    onClicked: {
                        if (audioPlayer.volume > 0) {
                            previousVolume = audioPlayer.volume
                            audioPlayer.setVolume(0)
                        } else {
                            audioPlayer.setVolume(previousVolume > 0 ? previousVolume : 1.0)
                        }
                    }
                }
            }

            Slider {
                id: volumeSlider
                Layout.preferredWidth: 100
                from: 0.0
                to: 1.0
                value: audioPlayer.volume
                onMoved: {
                    audioPlayer.setVolume(value)
                }
                onPressedChanged: {
                    if (!pressed) {
                        audioPlayer.setVolume(value)
                    }
                }

                Connections {
                    target: audioPlayer
                    function onVolumeChanged() {
                        if (!volumeSlider.pressed) {
                            volumeSlider.value = audioPlayer.volume
                        }
                    }
                }

                background: Rectangle {
                    x: volumeSlider.leftPadding
                    y: volumeSlider.topPadding + volumeSlider.availableHeight / 2 - height / 2
                    implicitWidth: 100
                    implicitHeight: 4
                    width: volumeSlider.availableWidth
                    height: implicitHeight
                    radius: 2
                    color: "#2a2a35"

                    Rectangle {
                        width: volumeSlider.visualPosition * parent.width
                        height: parent.height
                        color: "#bd93f9"
                        radius: 2
                    }
                }

                handle: Rectangle {
                    x: volumeSlider.leftPadding + volumeSlider.visualPosition * (volumeSlider.availableWidth - width)
                    y: volumeSlider.topPadding + volumeSlider.availableHeight / 2 - height / 2
                    implicitWidth: 10
                    implicitHeight: 10
                    radius: 5
                    color: "#bd93f9"
                    visible: volumeSlider.hovered || volumeSlider.pressed
                }
            }
        }
    }
}
