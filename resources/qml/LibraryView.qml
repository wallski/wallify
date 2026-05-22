import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQml

Item {
    id: libraryView

    property var selectedPlaylist: null

    function formatDuration(ms) {
        if (!ms || ms <= 0) return "--:--";
        var totalSecs = Math.floor(ms / 1000);
        var seconds = totalSecs % 60;
        var minutes = Math.floor(totalSecs / 60);
        return minutes + ":" + (seconds < 10 ? "0" : "") + seconds;
    }

    function refreshSelectedPlaylist() {
        if (!selectedPlaylist) return;
        var currentId = selectedPlaylist.id;
        selectedPlaylist = null;
        for (var i = 0; i < localLibrary.playlists.length; ++i) {
            if (localLibrary.playlists[i].id === currentId) {
                selectedPlaylist = localLibrary.playlists[i];
                break;
            }
        }
    }

    Connections {
        target: localLibrary
        function onPlaylistsChanged() {
            refreshSelectedPlaylist()
        }
    }

    FileDialog {
        id: coverFileDialog
        title: "Select Playlist Cover Image"
        nameFilters: ["Image files (*.png *.jpg *.jpeg)"]
        onAccepted: {
            if (selectedPlaylist) {
                localLibrary.updatePlaylistCover(selectedPlaylist.id, selectedFile.toString())
                refreshSelectedPlaylist()
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 30
        spacing: 20

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: selectedPlaylist !== null
            spacing: 20

            RowLayout {
                Layout.fillWidth: true
                spacing: 15

                Rectangle {
                    width: 36
                    height: 36
                    radius: 18
                    color: backMouse.containsMouse ? "#2a2a35" : "#1e1e28"
                    border.color: "#2a2a35"
                    border.width: 1

                    Text {
                        anchors.centerIn: parent
                        text: ""
                        font.family: "Segoe MDL2 Assets"
                        font.pixelSize: 14
                        color: "#e2e2e2"
                    }

                    MouseArea {
                        id: backMouse
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: selectedPlaylist = null
                    }
                }

                Text {
                    text: "Back to Playlists"
                    color: "#89899f"
                    font.family: "Roboto"
                    font.pixelSize: 14
                    font.bold: true
                }

                Item { Layout.fillWidth: true }

                Rectangle {
                    id: scanButton
                    width: 140
                    height: 40
                    color: scanMouse.pressed ? "#9a6fd6" : (scanMouse.containsMouse ? "#ab7fe6" : "#bd93f9")
                    radius: 20
                    visible: selectedPlaylist && selectedPlaylist.isDefault

                    Behavior on color { ColorAnimation { duration: 150 } }

                    RowLayout {
                        anchors.centerIn: parent
                        spacing: 8

                        Text {
                            text: ""
                            font.family: "Segoe MDL2 Assets"
                            font.pixelSize: 14
                            color: "#18181e"
                            font.bold: true
                        }

                        Text {
                            text: "SCAN FOLDER"
                            font.family: "Roboto"
                            font.pixelSize: 12
                            font.bold: true
                            color: "#18181e"
                        }
                    }

                    MouseArea {
                        id: scanMouse
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            localLibrary.scan()
                        }
                    }
                }

                Rectangle {
                    width: 140
                    height: 40
                    color: deleteMouse.containsMouse ? "#ff5555" : "#1e1e28"
                    border.color: deleteMouse.containsMouse ? "#ff5555" : "#3f3f52"
                    border.width: 1
                    radius: 20
                    visible: selectedPlaylist && !selectedPlaylist.isDefault

                    Behavior on color { ColorAnimation { duration: 150 } }

                    RowLayout {
                        anchors.centerIn: parent
                        spacing: 8

                        Text {
                            text: ""
                            font.family: "Segoe MDL2 Assets"
                            font.pixelSize: 14
                            color: deleteMouse.containsMouse ? "#18181e" : "#ff5555"
                        }

                        Text {
                            text: "DELETE PLAYLIST"
                            font.family: "Roboto"
                            font.pixelSize: 11
                            font.bold: true
                            color: deleteMouse.containsMouse ? "#18181e" : "#ff5555"
                        }
                    }

                    MouseArea {
                        id: deleteMouse
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            localLibrary.deletePlaylist(selectedPlaylist.id)
                            selectedPlaylist = null
                        }
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 25

                Rectangle {
                    Layout.preferredWidth: 140
                    Layout.preferredHeight: 140
                    color: "transparent"

                    Rectangle {
                        anchors.fill: parent
                        color: "#1e1e28"
                        radius: 8
                        clip: true
                        border.color: "#2a2a35"
                        border.width: 1

                        Image {
                            anchors.fill: parent
                            source: (selectedPlaylist && selectedPlaylist.coverPath) ? selectedPlaylist.coverPath : ""
                            fillMode: Image.PreserveAspectCrop
                            visible: selectedPlaylist && selectedPlaylist.coverPath !== ""
                        }

                        Rectangle {
                            anchors.fill: parent
                            color: "#23232f"
                            visible: !selectedPlaylist || selectedPlaylist.coverPath === ""

                            Text {
                                anchors.centerIn: parent
                                text: ""
                                font.family: "Segoe MDL2 Assets"
                                font.pixelSize: 42
                                color: "#4e4e5a"
                            }
                        }

                        Rectangle {
                            anchors.fill: parent
                            color: "#aa000000"
                            opacity: editCoverMouse.containsMouse ? 1.0 : 0.0
                            visible: opacity > 0.0
                            Behavior on opacity { NumberAnimation { duration: 150 } }

                            ColumnLayout {
                                anchors.centerIn: parent
                                spacing: 6

                                Text {
                                    text: ""
                                    font.family: "Segoe MDL2 Assets"
                                    font.pixelSize: 18
                                    color: "#e2e2e2"
                                    Layout.alignment: Qt.AlignHCenter
                                }

                                Text {
                                    text: "Choose Cover"
                                    font.family: "Roboto"
                                    font.pixelSize: 10
                                    font.bold: true
                                    color: "#e2e2e2"
                                    Layout.alignment: Qt.AlignHCenter
                                }
                            }

                            MouseArea {
                                id: editCoverMouse
                                anchors.fill: parent
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                onClicked: coverFileDialog.open()
                            }
                        }
                    }

                    Rectangle {
                        id: floatingEditBtn
                        width: 32
                        height: 32
                        radius: 16
                        color: editButtonMouse.containsMouse ? "#bd93f9" : "#2a2a35"
                        border.color: "#18181e"
                        border.width: 2
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        anchors.rightMargin: -8
                        anchors.bottomMargin: -8
                        z: 10

                        Behavior on color { ColorAnimation { duration: 150 } }

                        Text {
                            anchors.centerIn: parent
                            text: ""
                            font.family: "Segoe MDL2 Assets"
                            font.pixelSize: 13
                            color: editButtonMouse.containsMouse ? "#18181e" : "#e2e2e2"
                            font.bold: true
                        }

                        MouseArea {
                            id: editButtonMouse
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: coverFileDialog.open()
                        }
                    }
                }

                ColumnLayout {
                    spacing: 8
                    Layout.fillWidth: true

                    Text {
                        text: selectedPlaylist && selectedPlaylist.isDefault ? "DEFAULT PLAYLIST" : "PLAYLIST"
                        color: "#bd93f9"
                        font.family: "Roboto"
                        font.pixelSize: 11
                        font.bold: true
                        font.letterSpacing: 1.5
                    }

                    RowLayout {
                        spacing: 12
                        Layout.fillWidth: true

                        Text {
                            text: selectedPlaylist ? selectedPlaylist.name : ""
                            color: "#e2e2e2"
                            font.family: "Bebas Neue"
                            font.pixelSize: 38
                            font.letterSpacing: 1
                            visible: !titleEditField.visible
                        }

                        TextField {
                            id: titleEditField
                            text: selectedPlaylist ? selectedPlaylist.name : ""
                            color: "#e2e2e2"
                            font.family: "Bebas Neue"
                            font.pixelSize: 32
                            visible: false
                            focus: false
                            Layout.fillWidth: true
                            background: Rectangle {
                                color: "#18181e"
                                border.color: "#bd93f9"
                                border.width: 1
                                radius: 4
                            }
                            onEditingFinished: {
                                if (text.trim() !== "" && selectedPlaylist) {
                                    localLibrary.renamePlaylist(selectedPlaylist.id, text.trim())
                                    refreshSelectedPlaylist()
                                }
                                visible = false
                            }
                            Keys.onEscapePressed: visible = false
                        }

                        Rectangle {
                            width: 28
                            height: 28
                            radius: 14
                            color: editNameMouse.containsMouse ? "#2a2a35" : "transparent"
                            visible: !titleEditField.visible

                            Text {
                                anchors.centerIn: parent
                                text: ""
                                font.family: "Segoe MDL2 Assets"
                                font.pixelSize: 12
                                color: "#89899f"
                            }

                            MouseArea {
                                id: editNameMouse
                                anchors.fill: parent
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    titleEditField.visible = true
                                    titleEditField.focus = true
                                    titleEditField.selectAll()
                                }
                            }
                        }
                    }

                    Text {
                        text: (selectedPlaylist && selectedPlaylist.tracks ? selectedPlaylist.tracks.length : 0) + " songs"
                        color: "#89899f"
                        font.family: "Roboto"
                        font.pixelSize: 13
                    }
                }
            }

            // Play All + Shuffle buttons row
            RowLayout {
                Layout.fillWidth: true
                spacing: 12
                visible: selectedPlaylist && selectedPlaylist.tracks && selectedPlaylist.tracks.length > 0

                Rectangle {
                    width: 120
                    height: 36
                    color: playAllMouse.containsMouse ? "#ab7fe6" : "#bd93f9"
                    radius: 18

                    Behavior on color { ColorAnimation { duration: 150 } }

                    RowLayout {
                        anchors.centerIn: parent
                        spacing: 6

                        Text {
                            text: ""
                            font.family: "Segoe MDL2 Assets"
                            font.pixelSize: 14
                            color: "#18181e"
                        }

                        Text {
                            text: "PLAY ALL"
                            font.family: "Roboto"
                            font.pixelSize: 12
                            font.bold: true
                            color: "#18181e"
                        }
                    }

                    MouseArea {
                        id: playAllMouse
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            if (selectedPlaylist && selectedPlaylist.tracks && selectedPlaylist.tracks.length > 0) {
                                audioPlayer.playPlaylist(selectedPlaylist.tracks, false)
                            }
                        }
                    }
                }

                Rectangle {
                    width: 120
                    height: 36
                    color: shuffleAllMouse.containsMouse ? "#ab7fe6" : "#2a2a35"
                    border.color: "#bd93f9"
                    border.width: 1
                    radius: 18

                    Behavior on color { ColorAnimation { duration: 150 } }

                    RowLayout {
                        anchors.centerIn: parent
                        spacing: 6

                        Text {
                            text: ""
                            font.family: "Segoe MDL2 Assets"
                            font.pixelSize: 14
                            color: "#bd93f9"
                        }

                        Text {
                            text: "SHUFFLE"
                            font.family: "Roboto"
                            font.pixelSize: 12
                            font.bold: true
                            color: "#bd93f9"
                        }
                    }

                    MouseArea {
                        id: shuffleAllMouse
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            if (selectedPlaylist && selectedPlaylist.tracks && selectedPlaylist.tracks.length > 0) {
                                audioPlayer.playPlaylist(selectedPlaylist.tracks, true)
                            }
                        }
                    }
                }

                Item { Layout.fillWidth: true }

                // Sort dropdown
                Rectangle {
                    width: 140
                    height: 32
                    color: sortMouse.containsMouse ? "#2a2a35" : "#1e1e28"
                    border.color: "#3f3f52"
                    border.width: 1
                    radius: 16

                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: 12
                        anchors.rightMargin: 12
                        spacing: 6

                        Text {
                            text: ""
                            font.family: "Segoe MDL2 Assets"
                            font.pixelSize: 12
                            color: "#89899f"
                        }

                        Text {
                            text: localLibrary.sortMode === 0 ? "Custom" : (localLibrary.sortMode === 1 ? "Name" : "Album")
                            color: "#e2e2e2"
                            font.family: "Roboto"
                            font.pixelSize: 12
                            Layout.fillWidth: true
                        }
                    }

                    MouseArea {
                        id: sortMouse
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: sortMenu.visible = !sortMenu.visible
                    }
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true

                ColumnLayout {
                    anchors.centerIn: parent
                    spacing: 12
                    visible: !selectedPlaylist || !selectedPlaylist.tracks || selectedPlaylist.tracks.length === 0

                    Text {
                        text: ""
                        font.family: "Segoe MDL2 Assets"
                        font.pixelSize: 36
                        color: "#4e4e5a"
                        Layout.alignment: Qt.AlignHCenter
                    }

                    Text {
                        text: "This playlist is empty"
                        color: "#e2e2e2"
                        font.family: "Roboto"
                        font.pixelSize: 15
                        font.bold: true
                        Layout.alignment: Qt.AlignHCenter
                    }

                    Text {
                        text: selectedPlaylist && selectedPlaylist.isDefault ? 
                              "Click 'Scan Folder' above to search for MP3 files in your library path." :
                              "Right-click any song in your Library or other playlists and select 'Add to Playlist'."
                        color: "#89899f"
                        font.family: "Roboto"
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        Layout.alignment: Qt.AlignHCenter
                    }
                }

                ColumnLayout {
                    anchors.fill: parent
                    visible: selectedPlaylist && selectedPlaylist.tracks && selectedPlaylist.tracks.length > 0
                    spacing: 10

                    Rectangle {
                        Layout.fillWidth: true
                        height: 30
                        color: "transparent"

                        Rectangle {
                            width: parent.width
                            height: 1
                            color: "#2a2a35"
                            anchors.bottom: parent.bottom
                        }

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 15
                            anchors.rightMargin: 15
                            spacing: 10

                            Text {
                                text: "#"
                                color: "#89899f"
                                font.family: "Roboto"
                                font.pixelSize: 11
                                font.bold: true
                                Layout.minimumWidth: 30
                                Layout.maximumWidth: 30
                                Layout.preferredWidth: 30
                                verticalAlignment: Text.AlignVCenter
                            }

                            Text {
                                text: "TITLE"
                                color: "#89899f"
                                font.family: "Roboto"
                                font.pixelSize: 11
                                font.bold: true
                                Layout.fillWidth: true
                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideRight
                            }

                            Text {
                                text: "ALBUM"
                                color: "#89899f"
                                font.family: "Roboto"
                                font.pixelSize: 11
                                font.bold: true
                                Layout.minimumWidth: 200
                                Layout.maximumWidth: 200
                                Layout.preferredWidth: 200
                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideRight
                            }

                            Text {
                                text: ""
                                font.family: "Segoe MDL2 Assets"
                                font.pixelSize: 11
                                color: "#89899f"
                                Layout.minimumWidth: 90
                                Layout.maximumWidth: 90
                                Layout.preferredWidth: 90
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignRight
                            }
                        }
                    }

                    ListView {
                        id: trackList
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true
                        model: selectedPlaylist ? selectedPlaylist.tracks : []
                        spacing: 4
                        ScrollBar.vertical: ScrollBar {
                            policy: ScrollBar.AsNeeded
                        }

                        delegate: Rectangle {
                            id: trackRowItem
                            width: trackList.width
                            height: 56
                            property bool isContextMenuOpen: customTrackContextMenu.visible && customTrackContextMenu.trackData !== null && customTrackContextMenu.trackData.filePath === modelData.filePath
                            property bool isHovered: rowMouse.containsMouse || menuMouse.containsMouse || isContextMenuOpen
                            property var trackModelData: modelData
                            color: isHovered ? "#1e1e28" : "transparent"
                            radius: 4

                            Behavior on color { ColorAnimation { duration: 100 } }

                            MouseArea {
                                id: rowMouse
                                anchors.fill: parent
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                acceptedButtons: Qt.LeftButton | Qt.RightButton
                                onDoubleClicked: {
                                    if (mouse.button === Qt.LeftButton) {
                                        audioPlayer.playTrackFromPlaylist(modelData, selectedPlaylist.tracks)
                                    }
                                }
                                onClicked: {
                                    if (mouse.button === Qt.RightButton) {
                                        var globalCoords = rowMouse.mapToItem(libraryView, mouse.x, mouse.y)
                                        var targetX = globalCoords.x
                                        var targetY = globalCoords.y

                                        if (targetX + customTrackContextMenu.width > libraryView.width) {
                                            targetX = libraryView.width - customTrackContextMenu.width - 10
                                        }
                                        if (targetX < 10) {
                                            targetX = 10
                                        }
                                        if (targetY + customTrackContextMenu.height > libraryView.height) {
                                            targetY = libraryView.height - customTrackContextMenu.height - 10
                                        }
                                        if (targetY < 10) {
                                            targetY = 10
                                        }

                                        customTrackContextMenu.trackData = modelData
                                        customTrackContextMenu.x = targetX
                                        customTrackContextMenu.y = targetY
                                        customTrackContextMenu.visible = true
                                    }
                                }
                            }

                            RowLayout {
                                anchors.fill: parent
                                anchors.leftMargin: 15
                                anchors.rightMargin: 15
                                spacing: 10

                                Item {
                                    Layout.minimumWidth: 30
                                    Layout.maximumWidth: 30
                                    Layout.preferredWidth: 30
                                    Layout.fillHeight: true

                                    Text {
                                        text: (index + 1).toString()
                                        color: "#89899f"
                                        font.family: "Roboto"
                                        font.pixelSize: 13
                                        anchors.centerIn: parent
                                        visible: !isHovered && audioPlayer.currentTrack.filePath !== modelData.filePath
                                    }

                                    Text {
                                        text: audioPlayer.currentTrack.filePath === modelData.filePath && audioPlayer.isPlaying ? "" : ""
                                        font.family: "Segoe MDL2 Assets"
                                        font.pixelSize: 10
                                        color: "#bd93f9"
                                        anchors.centerIn: parent
                                        visible: isHovered || audioPlayer.currentTrack.filePath === modelData.filePath
                                    }
                                }

                                RowLayout {
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    spacing: 10

                                    Rectangle {
                                        width: 40
                                        height: 40
                                        color: "#2a2a35"
                                        radius: 4
                                        clip: true
                                        Layout.alignment: Qt.AlignVCenter

                                        Image {
                                            anchors.fill: parent
                                            source: modelData.hasCover ? "image://covers/" + encodeURIComponent(modelData.filePath) + "?t=" + modelData.coverUpdated : ""
                                            fillMode: Image.PreserveAspectCrop
                                            visible: modelData.hasCover
                                            cache: false
                                        }

                                        Rectangle {
                                            anchors.fill: parent
                                            color: "#23232f"
                                            visible: !modelData.hasCover

                                            Text {
                                                anchors.centerIn: parent
                                                text: ""
                                                font.family: "Segoe MDL2 Assets"
                                                font.pixelSize: 14
                                                color: "#4e4e5a"
                                            }
                                        }
                                    }

                                    ColumnLayout {
                                        spacing: 2
                                        Layout.fillWidth: true
                                        Layout.alignment: Qt.AlignVCenter

                                        Text {
                                            text: modelData.title
                                            color: audioPlayer.currentTrack.filePath === modelData.filePath ? "#bd93f9" : "#e2e2e2"
                                            font.family: "Roboto"
                                            font.pixelSize: 14
                                            font.bold: true
                                            elide: Text.ElideRight
                                            Layout.fillWidth: true
                                        }

                                        Text {
                                            text: modelData.artist
                                            color: "#89899f"
                                            font.family: "Roboto"
                                            font.pixelSize: 12
                                            elide: Text.ElideRight
                                            Layout.fillWidth: true
                                        }
                                    }
                                }

                                Text {
                                    text: modelData.album
                                    color: "#89899f"
                                    font.family: "Roboto"
                                    font.pixelSize: 13
                                    Layout.minimumWidth: 200
                                    Layout.maximumWidth: 200
                                    Layout.preferredWidth: 200
                                    Layout.alignment: Qt.AlignVCenter
                                    elide: Text.ElideRight
                                    verticalAlignment: Text.AlignVCenter
                                }

                                RowLayout {
                                    Layout.minimumWidth: 90
                                    Layout.maximumWidth: 90
                                    Layout.preferredWidth: 90
                                    Layout.fillHeight: true
                                    spacing: 10
                                    Layout.alignment: Qt.AlignVCenter

                                    Text {
                                        text: libraryView.formatDuration(modelData.duration)
                                        color: "#89899f"
                                        font.family: "Roboto"
                                        font.pixelSize: 13
                                        Layout.fillWidth: true
                                        horizontalAlignment: Text.AlignRight
                                        verticalAlignment: Text.AlignVCenter
                                    }

                                    Item {
                                        id: menuButtonContainer
                                        Layout.preferredWidth: 24
                                        Layout.preferredHeight: 24
                                        Layout.alignment: Qt.AlignVCenter
                                        opacity: isHovered ? 1.0 : 0.0
                                        Behavior on opacity { NumberAnimation { duration: 100 } }

                                        Rectangle {
                                            anchors.fill: parent
                                            radius: 12
                                            color: menuMouse.containsMouse ? "#2a2a35" : "transparent"
                                            Behavior on color { ColorAnimation { duration: 100 } }
                                        }

                                        Text {
                                            anchors.centerIn: parent
                                            text: ""
                                            font.family: "Segoe MDL2 Assets"
                                            font.pixelSize: 11
                                            color: menuMouse.containsMouse ? "#bd93f9" : "#89899f"
                                        }

                                        MouseArea {
                                            id: menuMouse
                                            anchors.fill: parent
                                            hoverEnabled: true
                                            acceptedButtons: Qt.LeftButton | Qt.RightButton
                                            cursorShape: Qt.PointingHandCursor
                                            onClicked: {
                                                var globalCoords = menuMouse.mapToItem(libraryView, mouse.x, mouse.y)
                                                var targetX = globalCoords.x
                                                var targetY = globalCoords.y

                                                targetX = targetX - customTrackContextMenu.width + menuButtonContainer.width
                                                targetY = targetY + menuButtonContainer.height + 4

                                                if (targetX + customTrackContextMenu.width > libraryView.width) {
                                                    targetX = libraryView.width - customTrackContextMenu.width - 10
                                                }
                                                if (targetX < 10) {
                                                    targetX = 10
                                                }
                                                if (targetY + customTrackContextMenu.height > libraryView.height) {
                                                    targetY = libraryView.height - customTrackContextMenu.height - 10
                                                }
                                                if (targetY < 10) {
                                                    targetY = 10
                                                }

                                                customTrackContextMenu.trackData = modelData
                                                customTrackContextMenu.x = targetX
                                                customTrackContextMenu.y = targetY
                                                customTrackContextMenu.visible = true
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: selectedPlaylist === null
            spacing: 20

            RowLayout {
                Layout.fillWidth: true

                ColumnLayout {
                    spacing: 4
                    Text {
                        text: "YOUR PLAYLISTS"
                        color: "#bd93f9"
                        font.family: "Roboto"
                        font.pixelSize: 12
                        font.bold: true
                        font.letterSpacing: 1.5
                    }

                    Text {
                        text: "Library & Playlists"
                        color: "#e2e2e2"
                        font.family: "Bebas Neue"
                        font.pixelSize: 42
                        font.letterSpacing: 1
                    }
                }
            }

            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true

                GridView {
                    id: playlistGrid
                    anchors.fill: parent
                    cellWidth: 200
                    cellHeight: 250
                    model: localLibrary.playlists

                    delegate: Item {
                        width: 180
                        height: 230

                        Rectangle {
                            id: plCardBg
                            anchors.fill: parent
                            color: plCardMouse.containsMouse ? "#1e1e28" : "#161620"
                            radius: 8
                            border.color: plCardMouse.containsMouse ? "#bd93f9" : "#2a2a35"
                            border.width: 1

                            Behavior on color { ColorAnimation { duration: 150 } }
                            Behavior on border.color { ColorAnimation { duration: 150 } }

                            ColumnLayout {
                                anchors.fill: parent
                                anchors.margins: 12
                                spacing: 10

                                Rectangle {
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 156
                                    color: "#2a2a35"
                                    radius: 6
                                    clip: true

                                    Image {
                                        anchors.fill: parent
                                        source: modelData.coverPath ? modelData.coverPath : ""
                                        fillMode: Image.PreserveAspectCrop
                                        visible: modelData.coverPath !== ""
                                    }

                                    Rectangle {
                                        anchors.fill: parent
                                        color: modelData.isDefault ? "#2a2a35" : "#23232f"
                                        visible: modelData.coverPath === ""

                                        Text {
                                            anchors.centerIn: parent
                                            text: modelData.isDefault ? "" : ""
                                            font.family: "Segoe MDL2 Assets"
                                            font.pixelSize: 38
                                            color: "#4e4e5a"
                                        }
                                    }

                                    Rectangle {
                                        anchors.fill: parent
                                        color: "#80000000"
                                        opacity: plCardMouse.containsMouse && modelData.tracks && modelData.tracks.length > 0 ? 1.0 : 0.0
                                        visible: opacity > 0.0
                                        Behavior on opacity { NumberAnimation { duration: 150 } }

                                        Rectangle {
                                            width: 44
                                            height: 44
                                            radius: 22
                                            color: "#bd93f9"
                                            anchors.centerIn: parent

                                            Text {
                                                anchors.centerIn: parent
                                                anchors.horizontalCenterOffset: 2
                                                text: ""
                                                font.family: "Segoe MDL2 Assets"
                                                font.pixelSize: 16
                                                color: "#18181e"
                                            }

                                            MouseArea {
                                                anchors.fill: parent
                                                cursorShape: Qt.PointingHandCursor
                                                onClicked: {
                                                    if (modelData.tracks && modelData.tracks.length > 0) {
                                                        audioPlayer.playPlaylist(modelData.tracks, false)
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }

                                ColumnLayout {
                                    Layout.fillWidth: true
                                    spacing: 2

                                    Text {
                                        text: modelData.name
                                        color: "#e2e2e2"
                                        font.family: "Roboto"
                                        font.bold: true
                                        font.pixelSize: 14
                                        elide: Text.ElideRight
                                        Layout.fillWidth: true
                                    }

                                    Text {
                                        text: modelData.isDefault ? "Default • Scanned Library" : "Custom Playlist"
                                        color: "#89899f"
                                        font.family: "Roboto"
                                        font.pixelSize: 12
                                        elide: Text.ElideRight
                                        Layout.fillWidth: true
                                    }
                                }
                            }

                            MouseArea {
                                id: plCardMouse
                                anchors.fill: parent
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    selectedPlaylist = modelData
                                }
                            }
                        }
                    }

                    footer: Item {
                        width: 180
                        height: 230

                        Rectangle {
                            id: createPlCard
                            anchors.fill: parent
                            anchors.margins: 0
                            color: createPlMouse.containsMouse ? "#1e1e28" : "#161620"
                            radius: 8
                            border.color: createPlMouse.containsMouse ? "#bd93f9" : "#2a2a35"
                            border.width: 1

                            ColumnLayout {
                                anchors.centerIn: parent
                                spacing: 12

                                Rectangle {
                                    width: 50
                                    height: 50
                                    radius: 25
                                    color: createPlMouse.containsMouse ? "#bd93f9" : "#2a2a35"
                                    Layout.alignment: Qt.AlignHCenter
                                    Behavior on color { ColorAnimation { duration: 150 } }

                                    Text {
                                        anchors.centerIn: parent
                                        text: ""
                                        font.family: "Segoe MDL2 Assets"
                                        font.pixelSize: 16
                                        color: createPlMouse.containsMouse ? "#18181e" : "#89899f"
                                    }
                                }

                                Text {
                                    text: "Create Playlist"
                                    color: "#e2e2e2"
                                    font.family: "Roboto"
                                    font.pixelSize: 14
                                    font.bold: true
                                    Layout.alignment: Qt.AlignHCenter
                                }
                            }

                            MouseArea {
                                id: createPlMouse
                                anchors.fill: parent
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                onClicked: createPlaylistDialog.visible = true
                            }
                        }
                    }
                }
            }
        }
    }

    MouseArea {
        id: customMenuDismissOverlay
        anchors.fill: parent
        enabled: customTrackContextMenu.visible || sortMenu.visible || renameDialog.visible
        visible: customTrackContextMenu.visible || sortMenu.visible || renameDialog.visible
        z: 999
        acceptedButtons: Qt.LeftButton | Qt.RightButton | Qt.MiddleButton
        onPressed: {
            customTrackContextMenu.visible = false
            sortMenu.visible = false
            renameDialog.visible = false
        }
    }

    Rectangle {
        id: customTrackContextMenu
        width: 190
        height: visible ? (menuItemRemove.visible ? 136 : 102) : 0
        color: "#1e1e28"
        border.color: "#3f3f52"
        border.width: 1
        radius: 6
        visible: false
        z: 1000

        property var trackData: null

        Behavior on height { NumberAnimation { duration: 120; easing.type: Easing.OutCubic } }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 4
            spacing: 2

            Rectangle {
                id: menuItemQueue
                Layout.fillWidth: true
                height: 30
                color: queueMouse.containsMouse ? "#2a2a35" : "transparent"
                radius: 4

                property bool containsMouse: queueMouse.containsMouse

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 12
                    anchors.rightMargin: 12
                    spacing: 10

                    Text {
                        text: ""
                        font.family: "Segoe MDL2 Assets"
                        font.pixelSize: 11
                        color: menuItemQueue.containsMouse ? "#bd93f9" : "#e2e2e2"
                    }

                    Text {
                        text: "Add to Queue"
                        font.family: "Roboto"
                        font.pixelSize: 13
                        color: menuItemQueue.containsMouse ? "#bd93f9" : "#e2e2e2"
                        Layout.fillWidth: true
                    }
                }

                MouseArea {
                    id: queueMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        if (customTrackContextMenu.trackData) {
                            audioPlayer.addToQueue(customTrackContextMenu.trackData)
                        }
                        customTrackContextMenu.visible = false
                    }
                }
            }

            Rectangle {
                id: menuItemPlaylist
                Layout.fillWidth: true
                height: 30
                color: playlistMouse.containsMouse ? "#2a2a35" : "transparent"
                radius: 4

                property bool containsMouse: playlistMouse.containsMouse

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 12
                    anchors.rightMargin: 12
                    spacing: 10

                    Text {
                        text: ""
                        font.family: "Segoe MDL2 Assets"
                        font.pixelSize: 11
                        color: menuItemPlaylist.containsMouse ? "#bd93f9" : "#e2e2e2"
                    }

                    Text {
                        text: "Add to Playlist..."
                        font.family: "Roboto"
                        font.pixelSize: 13
                        color: menuItemPlaylist.containsMouse ? "#bd93f9" : "#e2e2e2"
                        Layout.fillWidth: true
                    }
                }

                MouseArea {
                    id: playlistMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        if (customTrackContextMenu.trackData) {
                            addToPlaylistDialog.trackData = customTrackContextMenu.trackData
                            addToPlaylistDialog.visible = true
                        }
                        customTrackContextMenu.visible = false
                    }
                }
            }

            Rectangle {
                id: menuItemRename
                Layout.fillWidth: true
                height: 30
                color: renameMouse.containsMouse ? "#2a2a35" : "transparent"
                radius: 4

                property bool containsMouse: renameMouse.containsMouse

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 12
                    anchors.rightMargin: 12
                    spacing: 10

                    Text {
                        text: ""
                        font.family: "Segoe MDL2 Assets"
                        font.pixelSize: 11
                        color: menuItemRename.containsMouse ? "#bd93f9" : "#e2e2e2"
                    }

                    Text {
                        text: "Rename Song"
                        font.family: "Roboto"
                        font.pixelSize: 13
                        color: menuItemRename.containsMouse ? "#bd93f9" : "#e2e2e2"
                        Layout.fillWidth: true
                    }
                }

                MouseArea {
                    id: renameMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        if (customTrackContextMenu.trackData) {
                            renameDialog.trackData = customTrackContextMenu.trackData
                            renameDialog.visible = true
                        }
                        customTrackContextMenu.visible = false
                    }
                }
            }

            Rectangle {
                id: menuItemRemove
                Layout.fillWidth: true
                height: 30
                color: removeMouse.containsMouse ? "#2a2a35" : "transparent"
                radius: 4
                visible: selectedPlaylist !== null && !selectedPlaylist.isDefault

                property bool containsMouse: removeMouse.containsMouse

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 12
                    anchors.rightMargin: 12
                    spacing: 10

                    Text {
                        text: ""
                        font.family: "Segoe MDL2 Assets"
                        font.pixelSize: 11
                        color: "#ff5555"
                    }

                    Text {
                        text: "Remove from Playlist"
                        font.family: "Roboto"
                        font.pixelSize: 13
                        color: "#ff5555"
                        Layout.fillWidth: true
                    }
                }

                MouseArea {
                    id: removeMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        if (customTrackContextMenu.trackData && selectedPlaylist) {
                            localLibrary.removeTrackFromPlaylist(selectedPlaylist.id, customTrackContextMenu.trackData.filePath)
                            refreshSelectedPlaylist()
                        }
                        customTrackContextMenu.visible = false
                    }
                }
            }
        }
    }

    Rectangle {
        id: addToPlaylistDialog
        anchors.fill: parent
        color: "#d0000000"
        visible: false
        z: 2000

        property var trackData: null

        MouseArea {
            anchors.fill: parent
            onClicked: addToPlaylistDialog.visible = false
        }

        Rectangle {
            width: 380
            height: 320
            color: "#1e1e28"
            radius: 12
            border.color: "#3f3f52"
            border.width: 1
            anchors.centerIn: parent

            MouseArea { anchors.fill: parent }

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 12

                Text {
                    text: "ADD SONG TO PLAYLIST"
                    color: "#bd93f9"
                    font.family: "Roboto"
                    font.pixelSize: 12
                    font.bold: true
                    font.letterSpacing: 1.5
                }

                Text {
                    text: addToPlaylistDialog.trackData ? addToPlaylistDialog.trackData.title : ""
                    color: "#e2e2e2"
                    font.family: "Roboto"
                    font.pixelSize: 15
                    font.bold: true
                    elide: Text.ElideRight
                    Layout.fillWidth: true
                }

                ScrollView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true

                    ColumnLayout {
                        width: parent.width
                        spacing: 6

                        Repeater {
                            model: localLibrary.playlists
                            delegate: Rectangle {
                                Layout.fillWidth: true
                                height: 38
                                color: playlistItemMouse.containsMouse ? "#2a2a35" : "transparent"
                                radius: 6
                                visible: modelData.id !== "default"

                                RowLayout {
                                    anchors.fill: parent
                                    anchors.leftMargin: 12
                                    anchors.rightMargin: 12
                                    spacing: 12

                                    Text {
                                        text: ""
                                        font.family: "Segoe MDL2 Assets"
                                        font.pixelSize: 12
                                        color: playlistItemMouse.containsMouse ? "#bd93f9" : "#89899f"
                                    }

                                    Text {
                                        text: modelData.name
                                        color: playlistItemMouse.containsMouse ? "#ffffff" : "#e2e2e2"
                                        font.family: "Roboto"
                                        font.pixelSize: 13
                                        Layout.fillWidth: true
                                    }
                                }

                                MouseArea {
                                    id: playlistItemMouse
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    cursorShape: Qt.PointingHandCursor
                                    onClicked: {
                                        if (addToPlaylistDialog.trackData) {
                                            localLibrary.addTrackToPlaylist(modelData.id, addToPlaylistDialog.trackData.filePath)
                                        }
                                        addToPlaylistDialog.visible = false
                                    }
                                }
                            }
                        }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignRight
                    spacing: 12

                    Rectangle {
                        width: 80
                        height: 32
                        color: "transparent"
                        border.color: "#3f3f52"
                        border.width: 1
                        radius: 16

                        Text {
                            anchors.centerIn: parent
                            text: "Close"
                            color: "#89899f"
                            font.family: "Roboto"
                            font.pixelSize: 12
                            font.bold: true
                        }

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: addToPlaylistDialog.visible = false
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        id: createPlaylistDialog
        anchors.fill: parent
        color: "#d0000000"
        visible: false

        MouseArea {
            anchors.fill: parent
            onClicked: createPlaylistDialog.visible = false
        }

        Rectangle {
            width: 350
            height: 180
            color: "#1e1e28"
            radius: 12
            border.color: "#3f3f52"
            border.width: 1
            anchors.centerIn: parent

            MouseArea { anchors.fill: parent }

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 15

                Text {
                    text: "Create New Playlist"
                    color: "#e2e2e2"
                    font.family: "Roboto"
                    font.pixelSize: 18
                    font.bold: true
                }

                TextField {
                    id: createNameField
                    Layout.fillWidth: true
                    color: "#e2e2e2"
                    placeholderText: "Playlist Name"
                    background: Rectangle {
                        color: "#18181e"
                        border.color: createNameField.activeFocus ? "#bd93f9" : "#2a2a35"
                        border.width: 1
                        radius: 6
                    }
                    Keys.onReturnPressed: {
                        if (createNameField.text.trim() !== "") {
                            localLibrary.createPlaylist(createNameField.text.trim())
                            createPlaylistDialog.visible = false
                            createNameField.text = ""
                        }
                    }
                }

                RowLayout {
                    Layout.alignment: Qt.AlignRight
                    spacing: 12

                    Rectangle {
                        width: 80
                        height: 32
                        color: "transparent"
                        border.color: "#3f3f52"
                        border.width: 1
                        radius: 16

                        Text {
                            anchors.centerIn: parent
                            text: "Cancel"
                            color: "#89899f"
                            font.family: "Roboto"
                            font.pixelSize: 12
                            font.bold: true
                        }

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: createPlaylistDialog.visible = false
                        }
                    }

                    Rectangle {
                        width: 80
                        height: 32
                        color: createBtnMouse.containsMouse ? "#ab7fe6" : "#bd93f9"
                        radius: 16

                        Text {
                            anchors.centerIn: parent
                            text: "Create"
                            color: "#18181e"
                            font.family: "Roboto"
                            font.pixelSize: 12
                            font.bold: true
                        }

                        MouseArea {
                            id: createBtnMouse
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                if (createNameField.text.trim() !== "") {
                                    localLibrary.createPlaylist(createNameField.text.trim())
                                    createPlaylistDialog.visible = false
                                    createPlaylistDialog.text = ""
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Sort Menu
    Rectangle {
        id: sortMenu
        width: 160
        height: visible ? 96 : 0
        color: "#1e1e28"
        border.color: "#3f3f52"
        border.width: 1
        radius: 6
        visible: false
        z: 1001
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 200
        anchors.rightMargin: 30

        Behavior on height { NumberAnimation { duration: 120; easing.type: Easing.OutCubic } }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 4
            spacing: 2

            Rectangle {
                Layout.fillWidth: true
                height: 28
                color: sortCustomMouse.containsMouse ? "#2a2a35" : "transparent"
                radius: 4

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 12
                    anchors.rightMargin: 12
                    spacing: 10

                    Text {
                        text: ""
                        font.family: "Segoe MDL2 Assets"
                        font.pixelSize: 11
                        color: localLibrary.sortMode === 0 ? "#bd93f9" : "#e2e2e2"
                    }

                    Text {
                        text: "Custom (Date Added)"
                        font.family: "Roboto"
                        font.pixelSize: 12
                        color: localLibrary.sortMode === 0 ? "#bd93f9" : "#e2e2e2"
                        Layout.fillWidth: true
                    }
                }

                MouseArea {
                    id: sortCustomMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        localLibrary.setSortMode(0)
                        sortMenu.visible = false
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                height: 28
                color: sortNameMouse.containsMouse ? "#2a2a35" : "transparent"
                radius: 4

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 12
                    anchors.rightMargin: 12
                    spacing: 10

                    Text {
                        text: ""
                        font.family: "Segoe MDL2 Assets"
                        font.pixelSize: 11
                        color: localLibrary.sortMode === 1 ? "#bd93f9" : "#e2e2e2"
                    }

                    Text {
                        text: "Name (A-Z)"
                        font.family: "Roboto"
                        font.pixelSize: 12
                        color: localLibrary.sortMode === 1 ? "#bd93f9" : "#e2e2e2"
                        Layout.fillWidth: true
                    }
                }

                MouseArea {
                    id: sortNameMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        localLibrary.setSortMode(1)
                        sortMenu.visible = false
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                height: 28
                color: sortAlbumMouse.containsMouse ? "#2a2a35" : "transparent"
                radius: 4

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 12
                    anchors.rightMargin: 12
                    spacing: 10

                    Text {
                        text: ""
                        font.family: "Segoe MDL2 Assets"
                        font.pixelSize: 11
                        color: localLibrary.sortMode === 2 ? "#bd93f9" : "#e2e2e2"
                    }

                    Text {
                        text: "Album"
                        font.family: "Roboto"
                        font.pixelSize: 12
                        color: localLibrary.sortMode === 2 ? "#bd93f9" : "#e2e2e2"
                        Layout.fillWidth: true
                    }
                }

                MouseArea {
                    id: sortAlbumMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        localLibrary.setSortMode(2)
                        sortMenu.visible = false
                    }
                }
            }
        }
    }

    // Rename Song Dialog
    Rectangle {
        id: renameDialog
        anchors.fill: parent
        color: "#d0000000"
        visible: false
        z: 2000

        property var trackData: null

        MouseArea {
            anchors.fill: parent
            onClicked: renameDialog.visible = false
        }

        Rectangle {
            width: 380
            height: 200
            color: "#1e1e28"
            radius: 12
            border.color: "#3f3f52"
            border.width: 1
            anchors.centerIn: parent

            MouseArea { anchors.fill: parent }

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 15

                Text {
                    text: "RENAME SONG"
                    color: "#bd93f9"
                    font.family: "Roboto"
                    font.pixelSize: 12
                    font.bold: true
                    font.letterSpacing: 1.5
                }

                Text {
                    text: renameDialog.trackData ? renameDialog.trackData.title : ""
                    color: "#89899f"
                    font.family: "Roboto"
                    font.pixelSize: 13
                    elide: Text.ElideRight
                    Layout.fillWidth: true
                }

                TextField {
                    id: renameField
                    Layout.fillWidth: true
                    color: "#e2e2e2"
                    placeholderText: "New Song Title"
                    background: Rectangle {
                        color: "#18181e"
                        border.color: renameField.activeFocus ? "#bd93f9" : "#2a2a35"
                        border.width: 1
                        radius: 6
                    }
                    Keys.onReturnPressed: {
                        if (renameField.text.trim() !== "" && renameDialog.trackData) {
                            localLibrary.renameTrack(renameDialog.trackData.filePath, renameField.text.trim())
                            renameDialog.visible = false
                            renameField.text = ""
                        }
                    }
                }

                RowLayout {
                    Layout.alignment: Qt.AlignRight
                    spacing: 12

                    Rectangle {
                        width: 80
                        height: 32
                        color: "transparent"
                        border.color: "#3f3f52"
                        border.width: 1
                        radius: 16

                        Text {
                            anchors.centerIn: parent
                            text: "Cancel"
                            color: "#89899f"
                            font.family: "Roboto"
                            font.pixelSize: 12
                            font.bold: true
                        }

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: renameDialog.visible = false
                        }
                    }

                    Rectangle {
                        width: 80
                        height: 32
                        color: renameBtnMouse.containsMouse ? "#ab7fe6" : "#bd93f9"
                        radius: 16

                        Text {
                            anchors.centerIn: parent
                            text: "Rename"
                            color: "#18181e"
                            font.family: "Roboto"
                            font.pixelSize: 12
                            font.bold: true
                        }

                        MouseArea {
                            id: renameBtnMouse
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                if (renameField.text.trim() !== "" && renameDialog.trackData) {
                                    localLibrary.renameTrack(renameDialog.trackData.filePath, renameField.text.trim())
                                    renameDialog.visible = false
                                    renameField.text = ""
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
