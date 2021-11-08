@echo off

:: Can be either "x86" or "amd64"
set Architecture=amd64
:: Can be either "Debug" or "Release"
set BuildMode=Debug

set Libraries=SDL2main.lib SDL2.lib SDL2_mixer.lib user32.lib shell32.lib setargv.obj

set SDL2LibArch=

if %Architecture%==x86 set SDL2LibArch=x86
if %Architecture%==amd64 set SDL2LibArch=x64

set SDL2BinDir=source\Depends\SDL2\lib\win32\%SDL2LibArch%
set SDL2MixerBinDir=source\Depends\SDL2_mixer\lib\win32\%SDL2LibArch%

set IncludeDirs=-I source\Depends\SDL2\include\SDL2 -I source\Depends\SDL2\include -I source\Depends\SDL2_mixer\include -I source\Depends\STB
set LibraryDirs=-libpath:%SDL2BinDir% -libpath:%SDL2MixerBinDir%

set Defines=-D PLATFORM_WIN32 -D _CRT_SECURE_NO_WARNINGS

set CompilerFlags=-Zc:__cplusplus -std:c++17 -nologo -W4 -MT -Oi -EHsc -Z7
set LinkerFlags=-opt:ref -incremental:no

set CompilerWarnings=-wd4100 -wd4505 -wd4189
set LinkerWarnings=-ignore:4099

set ResourceFile=resource\resources.rc
set ResourcePath=resource\

set InputResource=resource\resources.res
set InputSource=source\main.cpp

set OutputPath=binary\win32\
set OutputName=RUNNER

if %BuildMode%==Release (
    set CompilerFlags=%CompilerFlags%
    set LinkerFlags=%LinkerFlags% -subsystem:windows
)
if %BuildMode%==Debug (
    set InputResource=
    set LinkerFlags=%LinkerFlags% -subsystem:console
)

if %Architecture%==x86 (
    set CompilerFlags=%CompilerFlags% -arch:IA32
    set LinkerFlags=%LinkerFlags%,5.1
)

set OutputExecutable=%OutputPath%%OutputName%
