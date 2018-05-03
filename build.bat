@echo off
REM wget https://aka.ms/vs/15/release
REM wget http://download.microsoft.com/download/8/6/3/86383155-3307-43F2-B7C9-F4F9EC213623/visualstudio.vsman
set SYS_DIR=%USERPROFILE%\Downloads\BuildSystem
set PATH=%PATH%;%SYS_DIR%\bin
set SYS_INCLUDE_DIR=/I %SYS_DIR%\include\msvc /I %SYS_DIR%\include\shared /I %SYS_DIR%\include\ucrt /I %SYS_DIR%\include\um
cl.exe -c %SYS_INCLUDE_DIR% vcinstaller.cpp
REM must not use vcinstaller.exe or the executable will require admin privilege
link.exe /libpath:%SYS_DIR%\lib /subsystem:Console vcinstaller.obj /out:a.exe
