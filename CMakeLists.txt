cmake_minimum_required(VERSION 3.16)

project(wallify VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configure CMake to use the local Qt installation
if(NOT DEFINED CMAKE_PREFIX_PATH)
    set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/deps/6.8.2/msvc2022_64" CACHE PATH "Path to Qt" FORCE)
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick Multimedia)

qt_standard_project_setup()

qt_add_executable(wallify
    MANUAL_FINALIZATION
    src/main.cpp
    src/SpotifyMigrator.cpp
    src/SpotifyMigrator.h
    src/YouTubeMigrator.cpp
    src/YouTubeMigrator.h
    src/SoundCloudMigrator.cpp
    src/SoundCloudMigrator.h
    src/DiscordRPC.cpp
    src/DiscordRPC.h
    src/AppSettings.cpp
    src/AppSettings.h
    src/LocalLibrary.cpp
    src/LocalLibrary.h
    src/AudioPlayer.cpp
    src/AudioPlayer.h
    src/CoverImageProvider.h
)

# Discord RPC static library (bundled, no extra DLL needed)
# Place discord_rpc.lib in src/discord-rpc/lib/ and headers in src/discord-rpc/include/
target_link_libraries(wallify
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/discord-rpc/lib/discord-rpc.lib
)

# Include Discord RPC headers
target_include_directories(wallify
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/discord-rpc/include
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
        resources/qml/LibraryView.qml
        resources/qml/DebugWindow.qml
        resources/qml/MigrationView.qml
        resources/qml/Sidebar.qml
        resources/qml/BottomPlayerBar.qml
        resources/qml/SettingsView.qml
    RESOURCES
        resources/fonts/Roboto-Regular.ttf
        resources/fonts/BebasNeue-Regular.ttf
        resources/icons/app_icon.svg
)

target_link_libraries(wallify
    PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::Multimedia
)

set_target_properties(wallify PROPERTIES
    WIN32_EXECUTABLE TRUE
)

qt_import_qml_plugins(wallify)
qt_finalize_executable(wallify)

# --- Post-build: auto-deploy Qt DLLs with windeployqt ---
set(WINDEPLOYQT_EXE "${CMAKE_CURRENT_SOURCE_DIR}/deps/6.8.2/msvc2022_64/bin/windeployqt.exe")
set(QML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources/qml")

add_custom_command(TARGET wallify POST_BUILD
    COMMAND "${WINDEPLOYQT_EXE}"
            "$<TARGET_FILE:wallify>"
            --qmldir "${QML_DIR}"
    COMMENT "Running windeployqt..."
    VERBATIM
)
