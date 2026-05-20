# Wallify

## Overview
Wallify is a cross‑platform desktop music player built with Qt and C++. It provides a clean, modern interface for browsing and playing your local music library. The application supports automatic detection of playlists and albums, and it can import songs from external sources such as Spotify, YouTube, and SoundCloud.

## Features
- Elegant, responsive UI built with QML
- Full‑screen and windowed modes
- Automatic playlist detection and creation
- Migration tool that can import tracks and playlists from multiple services
- Local library management with automatic scanning and metadata handling
- Configurable settings for themes, audio output, and shortcuts

## Prerequisites
- Windows 10/11 (64‑bit) or Linux with Qt 6 runtime
- CMake ≥ 3.20
- A C++ compiler compatible with C++17 (MSVC 2022, GCC 9+, Clang 10+)
- Qt 6.5+ libraries (installed via the included `deps` directory or system packages)

## Building from Source
```bash
# Clone the repository
git clone https://github.com/wallski/wallify.git
cd wallify

# Initialize submodules (Qt dependencies)
git submodule update --init --recursive

# Configure the build directory
cmake -S . -B build -G "Visual Studio 17 2022" -A x64

# Build the project
cmake --build build --config Release
```
The executable will be located at `build/Release/wallify.exe` (Windows) or `build/wallify` (Linux).

## Running the Application
Simply launch the built binary. The first run will prompt you to select a directory for your music library. Wallify will scan the folder and populate the library view.


## License
This project is licensed under the MIT License. See the `LICENSE` file for details.
