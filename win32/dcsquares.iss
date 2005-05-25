[Components]
Name: DCSquares; Description: DCSquares; Flags: fixed; Types: custom compact full
Name: default; Description: Default theme; Flags: fixed; Types: custom compact full
Name: classic_theme; Description: Classic theme; Types: full
[Icons]
Name: {group}\DCSquares; Filename: {app}\DCSquares.exe; WorkingDir: {app}; IconFilename: {app}\DCSquares.exe; IconIndex: 0
Name: {group}\ReadMe; Filename: {app}\readme.rtf
[Setup]
AppCopyright=©2005 Sam Steele
AppName=DCSquares
AppVerName=1.8
DefaultDirName={pf}\DCSquares
DefaultGroupName=DCSquares
ShowLanguageDialog=yes
AppPublisher=Sam Steele
AppPublisherURL=http://dcsquares.c99.org/2.0/
AppVersion=1.8
AppID={{2759251C-66AE-4578-84F4-8AD0560AC2E4}
UninstallDisplayIcon={app}\DCSquares.exe
UninstallDisplayName=DCSquares
WizardSmallImageFile=C:\DCSquares\icon.bmp
VersionInfoVersion=1.8.5.25
VersionInfoCompany=Sam Steele
VersionInfoTextVersion=DCSquares 1.8
InfoBeforeFile=
MinVersion=4.1.1998,5.0.2195
InternalCompressLevel=ultra
OutputBaseFilename=dcsquares-setup
Compression=lzma/ultra
Password=dcsqbeta525
[Run]
Filename: {app}\DCSquares.exe; WorkingDir: {app}; Description: Play DCSquares!; Flags: postinstall
[Tasks]
Name: ovcodec; Description: Install Ogg Vorbis DirectShow codecs
[Files]
Source: ..\data\Helvetica-Bold.txf; DestDir: {app}; Components: DCSquares
Source: ..\data\themes\classic\bg.png; DestDir: {app}\themes\classic; Components: classic_theme
Source: ..\data\themes\classic\bg_loop.ogg; DestDir: {app}\themes\classic; Components: classic_theme
Source: ..\data\themes\classic\bgdim_loop.ogg; DestDir: {app}\themes\classic; Components: classic_theme
Source: ..\data\themes\classic\collect.wav; DestDir: {app}\themes\classic; Components: classic_theme
Source: ..\data\themes\classic\gameover.wav; DestDir: {app}\themes\classic; Components: classic_theme
Source: ..\data\themes\classic\menu.png; DestDir: {app}\themes\classic; Components: classic_theme
Source: ..\data\themes\classic\powerdown.wav; DestDir: {app}\themes\classic; Components: classic_theme
Source: ..\data\themes\classic\powerup.wav; DestDir: {app}\themes\classic; Components: classic_theme
Source: ..\data\themes\classic\theme.ini; DestDir: {app}\themes\classic; Components: classic_theme
Source: ..\data\themes\classic\title.png; DestDir: {app}\themes\classic; Components: classic_theme
Source: ..\data\themes\default\title.png; DestDir: {app}\themes\default; Components: default
Source: ..\data\themes\default\bg.png; DestDir: {app}\themes\default; Components: default
Source: ..\data\themes\default\bg_loop.ogg; DestDir: {app}\themes\default; Components: default
Source: ..\data\themes\default\bgdim_loop.ogg; DestDir: {app}\themes\default; Components: default
Source: ..\data\themes\default\collect.wav; DestDir: {app}\themes\default; Components: default
Source: ..\data\themes\default\gameover.wav; DestDir: {app}\themes\default; Components: default
Source: ..\data\themes\default\green_invincible.png; DestDir: {app}\themes\default; Components: default
Source: ..\data\themes\default\green_plus1000.png; DestDir: {app}\themes\default; Components: default
Source: ..\data\themes\default\green_slowdown.png; DestDir: {app}\themes\default; Components: default
Source: ..\data\themes\default\green_smallsquare.png; DestDir: {app}\themes\default; Components: default
Source: ..\data\themes\default\green_square.png; DestDir: {app}\themes\default; Components: default
Source: ..\data\themes\default\loading.png; DestDir: {app}\themes\default; Components: default
Source: ..\data\themes\default\menu.png; DestDir: {app}\themes\default; Components: default
Source: ..\data\themes\default\orb_shadow.png; DestDir: {app}\themes\default; Components: default
Source: ..\data\themes\default\powerdown.wav; DestDir: {app}\themes\default; Components: default
Source: ..\data\themes\default\powerup.wav; DestDir: {app}\themes\default; Components: default
Source: ..\data\themes\default\red_bigsquare.png; DestDir: {app}\themes\default; Components: default
Source: ..\data\themes\default\red_evil.png; DestDir: {app}\themes\default; Components: default
Source: ..\data\themes\default\red_minus1000.png; DestDir: {app}\themes\default; Components: default
Source: ..\data\themes\default\red_speedup.png; DestDir: {app}\themes\default; Components: default
Source: ..\data\themes\default\red_square.png; DestDir: {app}\themes\default; Components: default
Source: ..\data\themes\default\theme.ini; DestDir: {app}\themes\default; Components: default
Source: Release\DCSquares-MFC.exe; DestDir: {app}; DestName: DCSquares.exe; Components: DCSquares
Source: dlls\vorbisenc.dll; DestDir: {sys}; Tasks: ovcodec; Flags: confirmoverwrite sharedfile
Source: dlls\MFC71.dll; DestDir: {app}; Components: DCSquares
Source: dlls\MFC71ENU.DLL; DestDir: {app}; Components: DCSquares
Source: dlls\msvcp71.dll; DestDir: {app}; Components: DCSquares
Source: dlls\msvcr71.dll; DestDir: {app}; Components: DCSquares
Source: dlls\ogg.dll; DestDir: {sys}; Tasks: ovcodec; Flags: confirmoverwrite sharedfile
Source: dlls\OggDS.dll; DestDir: {sys}; Tasks: ovcodec; Flags: regserver confirmoverwrite sharedfile
Source: dlls\vorbis.dll; DestDir: {sys}; Tasks: ovcodec; Flags: confirmoverwrite sharedfile
[Dirs]
Name: {app}\themes; Components: DCSquares
Name: {app}\themes\classic; Components: classic_theme
Name: {app}\themes\default; Components: default
