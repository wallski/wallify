# Wallify

Wallify is a native desktop music player built with Qt6 and C++. It focuses on speed, low resource usage, and a clean local-first experience for managing and playing your music library.

## Features

### Library Management
- Scan local music folders recursively
- Automatic metadata extraction:
  - Title
  - Artist
  - Album
  - Cover art
- Supports:
  - MP3
  - WAV
  - OGG

### Playlists & Queue
- Create, rename, and delete playlists
- Add or remove tracks from playlists
- Queue system with:
  - Shuffle
  - Repeat modes

### Spotify Migration
- Import playlists and tracks using `spotdl`
- Download and migrate Spotify libraries into your local collection

### User Interface
- Native desktop application
- Custom dark-themed UI built with QML
- Lightweight and responsive

---

# Tech Stack

- **Language:** C++20
- **Framework:** Qt 6.8
  - Core
  - GUI
  - QML
  - Quick
  - Multimedia
- **Build System:** CMake
- **Metadata Parsing:** Custom ID3v2 parser

Wallify does not rely on external tag parsing libraries.

---

# Building (Windows)

## Requirements

- Windows 10/11 (64-bit)
- Visual Studio 2022 or newer with C++ workload
- Python 3.12+
- CMake 3.20+

---

## Build Instructions

### Clone the Repository

```powershell
git clone https://github.com/wallski/wallify.git
cd wallify
```

### Install Dependencies

Installs:
- Qt 6.8.2
- FFmpeg
- spotdl

```powershell
.\install_deps.ps1
```

### Build the Project

```powershell
mkdir build
cd build

cmake .. -G "Visual Studio 18 2026" -A x64
cmake --build . --config Release
```

### Deploy Qt Dependencies

```powershell
cd Release
..\..\deps\6.8.2\msvc2022_64\bin\windeployqt.exe wallify.exe --qmldir ..\..\resources\qml
```

### Run

```powershell
.\wallify.exe
```

The executable will be located at:

```text
build/Release/wallify.exe
```

---

# Visual Studio Version Notes

Use the generator that matches your installed Visual Studio version.

## Visual Studio 2022

```powershell
cmake .. -G "Visual Studio 17 2022" -A x64
```

## Visual Studio 2026

```powershell
cmake .. -G "Visual Studio 18 2026" -A x64
```

If the generator does not match your installed version, CMake will fail during configuration.

---

# First Launch

When Wallify starts for the first time, it will ask for a music library folder.

Select the directory containing your music files and Wallify will automatically:
- Scan folders recursively
- Build the music library
- Extract metadata and cover art

---

# Known Bugs

- Downloaded songs lose the last character from the filename/title.
- Migrating a playlist or album incorrectly adds every song from the default library into the newly created playlist.

---

# Planned Features

## Migration
- YouTube support
- SoundCloud support
- Rename the Spotify migration tab to `Migration`
- Separate tabs for each migration service

## Playback & Library
- Rename songs from the right-click context menu
- Sorting options:
  - Name
  - Album
  - Custom order (date added)
- "Play Playlist" button
- Shuffle toggle for playlists

## Integrations
- Discord Rich Presence (RPC)

## UI / Branding
- Proper application icon and branding

---

# License

Licensed under the MIT License.
