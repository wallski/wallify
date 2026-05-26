# Wallify Build Script
$ErrorActionPreference = "Stop"

$ProjectDir = "C:\Users\elias\Desktop\GITHUB\wallify"

Write-Host ">> Entering project directory..." -ForegroundColor Cyan
Set-Location $ProjectDir

Write-Host ">> Deleting old build folder..." -ForegroundColor Yellow
if (Test-Path "build") {
    Remove-Item -Recurse -Force "build"
    Write-Host "   Build folder deleted." -ForegroundColor Green
} else {
    Write-Host "   No build folder found, skipping." -ForegroundColor DarkGray
}

Write-Host ">> Creating new build folder..." -ForegroundColor Cyan
New-Item -ItemType Directory -Name "build" | Out-Null
Set-Location "build"

Write-Host ">> Running CMake configure..." -ForegroundColor Cyan
cmake .. -G "Visual Studio 18 2026" -A x64
if ($LASTEXITCODE -ne 0) { Write-Host "CMake configure FAILED!" -ForegroundColor Red; exit 1 }

Write-Host ">> Building Release..." -ForegroundColor Cyan
cmake --build . --config Release
if ($LASTEXITCODE -ne 0) { Write-Host "CMake build FAILED!" -ForegroundColor Red; exit 1 }

Write-Host ">> Running windeployqt..." -ForegroundColor Cyan
Set-Location "Release"
..\..\deps\6.8.2\msvc2022_64\bin\windeployqt.exe wallify.exe --qmldir ..\..\resources\qml
if ($LASTEXITCODE -ne 0) { Write-Host "windeployqt FAILED!" -ForegroundColor Red; exit 1 }

Write-Host ""
Write-Host ">> Build complete! Wallify is ready in build\Release\" -ForegroundColor Green
