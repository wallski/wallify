# install_deps.ps1
# Script to install Qt dependencies for Wallify and download ffmpeg & spotdl

$ErrorActionPreference = "Stop"

# Ensure we are in the project root
$projectRoot = Split-Path -Parent $MyInvocation.MyCommand.Definition
Set-Location $projectRoot

# ---------- Python check ----------
# Check for python first, then py (Python Launcher on Windows)
$pythonCmd = $null
if (Get-Command "python" -ErrorAction SilentlyContinue) {
    $pythonCmd = "python"
} elseif (Get-Command "py" -ErrorAction SilentlyContinue) {
    $pythonCmd = "py"
} else {
    Write-Host "ERROR: Python is required to run aqtinstall." -ForegroundColor Red
    Write-Host "Install Python from https://python.org/downloads or run: py install 3.12" -ForegroundColor Yellow
    exit 1
}

Write-Host "Using Python command: $pythonCmd" -ForegroundColor Green

# ---------- Install aqtinstall (Qt downloader) ----------
Write-Host "Installing aqtinstall via pip..." -ForegroundColor Cyan
& $pythonCmd -m pip install --upgrade aqtinstall

# ---------- Download Qt ----------
$qtVersion = "6.8.2"
$destFolder = "deps"
Write-Host "Downloading Qt $qtVersion for MSVC 2022 64-bit..." -ForegroundColor Cyan
& $pythonCmd -m aqt install-qt windows desktop $qtVersion win64_msvc2022_64 -m qtmultimedia -O $destFolder

# ---------- Skip ffmpeg download (app will handle it at runtime) ----------
Write-Host "Skipping ffmpeg download. The application will download it when needed." -ForegroundColor Yellow

# ---------- Download spotdl ----------
$spotdlUrl = "https://github.com/spotDL/spotify-downloader/releases/download/v4.5.0/spotdl-4.5.0-win32.exe"
$spotdlDest = Join-Path $projectRoot "$destFolder\spotdl"
New-Item -ItemType Directory -Force -Path $spotdlDest | Out-Null
Write-Host "Downloading spotdl..." -ForegroundColor Cyan
Invoke-WebRequest -Uri $spotdlUrl -OutFile (Join-Path $spotdlDest "spotdl.exe")

Write-Host "All dependencies installed under '$destFolder'." -ForegroundColor Green