cmake_minimum_required(VERSION 3.16)

project(wallify VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configure CMake to use the local Qt installation
if(NOT DEFINED CMAKE_PREFIX_PATH)
    set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/deps/6.8.2/msvc2022_64" CACHE PATH "Path to Qt" FORCE)
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick)

qt_standard_project_setup()

qt_add_executable(wallify
    MANUAL_FINALIZATION
    src/main.cpp
    src/SpotifyMigrator.cpp
    src/SpotifyMigrator.h
    src/AppSettings.cpp
    src/AppSettings.h
)

# Create a QML module to embed QML files and resources
qt_add_qml_module(wallify
    URI Wallify
    VERSION 1.0
    RESOURCE_PREFIX /
    QML_FILES
        resources/qml/Main.qml
        resources/qml/TitleBar.qml
        resources/qml/WindowControlButton.qml
        resources/qml/WelcomeScreen.qml
        resources/qml/OnboardingView.qml
        resources/qml/MainInterface.qml
        resources/qml/MigrationView.qml
        resources/qml/Sidebar.qml
        resources/qml/BottomPlayerBar.qml
        resources/qml/SettingsView.qml
    RESOURCES
        resources/fonts/Roboto-Regular.ttf
        resources/fonts/BebasNeue-Regular.ttf
)

target_link_libraries(wallify
    PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
)

# set_target_properties(wallify PROPERTIES
#     WIN32_EXECUTABLE TRUE
# )

qt_import_qml_plugins(wallify)
qt_finalize_executable(wallify)
