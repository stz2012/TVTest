@echo off
setlocal enabledelayedexpansion

cd /d %~dp0\
if exist "%ProgramFiles%\Microsoft Visual Studio 14.0\Common7\Tools\vsvars32.bat" (
	call "%ProgramFiles%\Microsoft Visual Studio 14.0\Common7\Tools\vsvars32.bat"
	call devenv.com /rebuild "Release|Win32" TVTest.sln
)
if exist "%ProgramFiles(x86)%\Microsoft Visual Studio 14.0\Common7\Tools\vsvars32.bat" (
	call "%ProgramFiles(x86)%\Microsoft Visual Studio 14.0\Common7\Tools\vsvars32.bat"
	call devenv.com /rebuild "Release|Win32" TVTest.sln
)
pause
