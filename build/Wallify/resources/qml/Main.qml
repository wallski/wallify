import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Window
import QtQuick.Layouts

ApplicationWindow {
    id: mainWindow
    width: 1280
    height: 720
    visible: true
    title: qsTr("Wallify")

    // Remove Windows native title bar
    flags: Qt.Window | Qt.FramelessWindowHint

    color: "#0f0f13" 

    Connections {
        target: appSettings
        function onFactoryResetRequested() {
            stackView.replace(null, welcomeScreenComp)
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        TitleBar {
            Layout.fillWidth: true
            window: mainWindow
        }

        StackView {
            id: stackView
            Layout.fillWidth: true
            Layout.fillHeight: true
            initialItem: appSettings.isFirstRun ? welcomeScreenComp : mainInterfaceComp
            
            Component {
                id: welcomeScreenComp
                WelcomeScreen {
                    onGetStarted: {
                        stackView.push(onboardingViewComp)
                    }
                }
            }
            
            Component {
                id: onboardingViewComp
                OnboardingView {
                    onFinished: {
                        stackView.replace(null, mainInterfaceComp)
                    }
                }
            }
            
            Component {
                id: mainInterfaceComp
                MainInterface {}
            }
            
            replaceEnter: Transition {
                PropertyAnimation {
                    property: "opacity"
                    from: 0
                    to: 1
                    duration: 600
                    easing.type: Easing.OutCubic
                }
            }
            replaceExit: Transition {
                PropertyAnimation {
                    property: "opacity"
                    from: 1
                    to: 0
                    duration: 600
                    easing.type: Easing.InCubic
                }
            }
            pushEnter: Transition {
                PropertyAnimation {
                    property: "opacity"
                    from: 0
                    to: 1
                    duration: 600
                    easing.type: Easing.OutCubic
                }
            }
            pushExit: Transition {
                PropertyAnimation {
                    property: "opacity"
                    from: 1
                    to: 0
                    duration: 600
                    easing.type: Easing.InCubic
                }
            }
        }
    }
    
    DebugWindow {
        id: debugWindowItem
    }
}
