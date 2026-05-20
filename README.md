# Wallify

A native desktop music player built with Qt6 and C++. Fast and lightweight for playing your local music library.

## What it does

- Plays MP3, WAV, OGG from your local folders
- Scans your music directory and builds a library with metadata (title, artist, album, cover art)
- Playlist management: create, rename, delete, add/remove tracks
- Spotify migration: import playlists and tracks via spotdl
- Queue system with shuffle and repeat modes
- Custom dark UI built with QML

## Stack

- C++20
- Qt 6.8 (Core, Gui, QML, Quick, Multimedia)
- CMake
- Custom ID3v2 parser (no external tag library dependency)

## Building (Windows)

### Prerequisites

- Windows 10/11 (64-bit)
- Visual Studio 2022 or newer with C++ workload
- Python 3.12+ (for the dependency installer)
- CMake 3.20+

### Steps

```powershell
# Clone
git clone https://github.com/wallski/wallify.git
cd wallify

# Install Qt 6.8.2, FFmpeg, and spotdl into deps/
.\install_deps.ps1

# Build
mkdir build
cd build
cmake .. -G "Visual Studio 18 2026" -A x64
cmake --build . --config Release

# Deploy Qt dependencies (required to run)
cd Release
..\..\deps\6.8.2\msvc2022_64\bin\windeployqt.exe wallify.exe --qmldir ..\..\resources\qml

# Run
.\wallify.exe
```

The executable is at `build/Release/wallify.exe` after building.

### Note on Visual Studio version

If you have VS 2022, use `"Visual Studio 17 2022"`. If you have VS 2026, use `"Visual Studio 18 2026"`. CMake will error if the generator doesn't match your installed version.

## First Run

On launch, Wallify asks for a music library folder. Point it to wherever your MP3s live. It will scan recursively and build the library automatically.


## License

MIT
