@echo off

set Libraries=-s USE_SDL=2 -s USE_SDL_MIXER=2

set IncludeDirs=-I source\Depends\SDL2\include\SDL2 -I source\Depends\SDL2\include -I source\Depends\SDL2_mixer\include -I source\Depends\STB

set Defines=-D PLATFORM_WEB

set CompilerFlags=-std=c++17
set LinkerFlags=-s ALLOW_MEMORY_GROWTH=1

set AssetPath=assets

set InputSource=source\Main.cpp

set OutputPath=binary\web\
set OutputName=RUNNER

set OutputExecutable=%OutputPath%%OutputName%
