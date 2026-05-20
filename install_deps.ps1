# install_deps.ps1
# Script to install Qt dependencies for Wallify

$ErrorActionPreference = "Stop"

if (-not (Get-Command "python" -ErrorAction SilentlyContinue)) {
    Write-Host "ERROR: Python is required to run aqtinstall. Please install Python 3." -ForegroundColor Red
    exit 1
}

Write-Host "Installing aqtinstall via pip..." -ForegroundColor Cyan
python -m pip install aqtinstall

$qtVersion = "6.8.2"
$destFolder = "deps"

Write-Host "Downloading Qt $qtVersion for MSVC 2022 64-bit..." -ForegroundColor Cyan
python -m aqt install-qt windows desktop $qtVersion win64_msvc2022_64 -m qtmultimedia -O $destFolder

Write-Host "Dependencies successfully installed in '$destFolder'." -ForegroundColor Green
