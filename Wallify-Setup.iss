; Wallify Installer Script

#define MyAppName "Wallify"
#define MyAppVersion "1.0"
#define MyAppPublisher "Wallski"
#define MyAppExeName "wallify.exe"

[Setup]
AppId={{A3F1D2E4-7B6C-4A09-B8D3-2E5F1C9A0347}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={autopf}\{#MyAppName}
DisableProgramGroupPage=yes
PrivilegesRequiredOverridesAllowed=dialog
OutputDir=.
OutputBaseFilename=Wallify-Setup
SetupIconFile=C:\Users\elias\Desktop\GITHUB\wallify\wallify.ico
Compression=lzma2
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
; Main executable
Source: "build\Release\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion

; Qt + FFmpeg DLLs
Source: "build\Release\*.dll"; DestDir: "{app}"; Flags: ignoreversion

; Qt plugin folders (from windeployqt)
Source: "build\Release\generic\*";           DestDir: "{app}\generic";           Flags: ignoreversion recursesubdirs createallsubdirs
Source: "build\Release\iconengines\*";       DestDir: "{app}\iconengines";       Flags: ignoreversion recursesubdirs createallsubdirs
Source: "build\Release\imageformats\*";      DestDir: "{app}\imageformats";      Flags: ignoreversion recursesubdirs createallsubdirs
Source: "build\Release\multimedia\*";        DestDir: "{app}\multimedia";        Flags: ignoreversion recursesubdirs createallsubdirs
Source: "build\Release\networkinformation\*"; DestDir: "{app}\networkinformation"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "build\Release\platforms\*";         DestDir: "{app}\platforms";         Flags: ignoreversion recursesubdirs createallsubdirs
Source: "build\Release\qml\*";               DestDir: "{app}\qml";               Flags: ignoreversion recursesubdirs createallsubdirs
Source: "build\Release\qmltooling\*";        DestDir: "{app}\qmltooling";        Flags: ignoreversion recursesubdirs createallsubdirs
Source: "build\Release\tls\*";               DestDir: "{app}\tls";               Flags: ignoreversion recursesubdirs createallsubdirs
Source: "build\Release\translations\*";      DestDir: "{app}\translations";      Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}";  Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent