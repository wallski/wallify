import QtQuick
import QtQuick.Layouts

Item {
    id: mainInterface

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 0

            Sidebar {
                Layout.preferredWidth: 250
                Layout.fillHeight: true
                currentIndex: contentStack.currentIndex
                onShowLibrary: contentStack.currentIndex = 0
                onShowMigration: contentStack.currentIndex = 1
                onShowSettings: contentStack.currentIndex = 2
            }

            StackLayout {
                id: contentStack
                Layout.fillWidth: true
                Layout.fillHeight: true
                currentIndex: 1 // Start on Migration for testing

                // Index 0: Local Library
                Item {
                    Text {
                        anchors.centerIn: parent
                        text: "Local Library Area (Coming Soon)"
                        color: "#89899f"
                        font.family: "Roboto"
                    }
                }

                // Index 1: Migration Tool
                MigrationView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }

                // Index 2: Settings
                SettingsView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }
        }

        BottomPlayerBar {
            Layout.fillWidth: true
        }
    }
}
