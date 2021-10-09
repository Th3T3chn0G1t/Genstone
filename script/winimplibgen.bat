rem https://stackoverflow.com/questions/9946322/how-to-generate-an-import-library-lib-file-from-a-dll/39916789#39916789

@echo off
setlocal enabledelayedexpansion

for /f "usebackq tokens=*" %%i in (`vswhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do (
  set InstallDir=%%i
)

if exist "%InstallDir%\VC\Auxiliary\Build\Microsoft.VCToolsVersion.default.txt" (
  set /p Version=<"%InstallDir%\VC\Auxiliary\Build\Microsoft.VCToolsVersion.default.txt"
  set Version=!Version: =!
)

rem "%InstallDir%\VC\Tools\MSVC\%Version%\bin\HostX64\x64\dumpbin.exe"
rem for /f "tokens=1-4" %%1 in ('dumpbin /exports %1') do (
for /f "tokens=1-4" %%1 in ('"%InstallDir%\VC\Tools\MSVC\%Version%\bin\HostX64\x64\dumpbin.exe" /exports %1') do (
    set /a ordinal=%%1 2>nul
    set /a hint=0x%%2 2>nul
    set /a rva=0x%%3 2>nul
    if !ordinal! equ %%1 if !hint! equ 0x%%2 if !rva! equ 0x%%3 set exports=!exports! /export:%%4
)

for /f %%i in ("%1") do set dllpath=%%~dpni
start lib /out:%dllpath%.lib /def: %exports%
