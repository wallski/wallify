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
  - Repeat modes (off, repeat all, repeat one)
- "Play All" and "Shuffle" buttons on every playlist

### Migration
Import music from multiple platforms:
- **Spotify** — playlists, albums, and tracks via `spotdl`
- **YouTube** — videos and playlists via `yt-dlp`
- **SoundCloud** — tracks and sets via `scdl`

All downloaded songs are saved as MP3 with embedded metadata and cover art.

### Library Tools
- **Sort** songs by custom order (date added), name (A-Z), or album
- **Rename** songs directly from the right-click context menu (updates ID3 tags)
- Playlist cover image selection

### Integrations
- **Discord Rich Presence** — shows what you're listening to on your Discord profile with live timestamps

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
- **Discord RPC:** Static-linked `discord-rpc` library

Wallify does not rely on external tag parsing libraries.

---

# Building (Windows)

## Requirements

- Windows 10/11 (64-bit)
- Visual Studio 2022 or newer with C++ workload
- Python 3.12+
- CMake 3.20+

---

## Setup

### Clone the Repository

```powershell
git clone https://github.com/wallski/wallify.git
cd wallify
```

### Install Dependencies

Installs Qt 6.8.2, FFmpeg, and spotdl:

```powershell
.\\install_deps.ps1
```
---

## Build Instructions

```powershell
mkdir build
cd build

cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

### Deploy Qt Dependencies

```powershell
cd Release
..\\..\\deps\\6.8.2\\msvc2022_64\\bin\\windeployqt.exe wallify.exe --qmldir ..\\..\\resources\\qml
```

### Run

```powershell
.\\wallify.exe
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

- Downloaded songs may lose the last character from the filename when using `spotdl` (upstream issue, not Wallify)

---

# Planned Features

- WAV and OGG playback support (currently MP3 only)
- Cross-platform Linux and macOS builds
- Mini player mode
- System tray integration

---

# License

Licensed under the MIT License.
