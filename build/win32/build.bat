@echo off
setlocal

call findvs.bat
call config.bat

call %VSDevPath% -no_logo -arch=%Architecture%

if not exist %OutputPath% mkdir %OutputPath%

if not exist %OutputPath%\SDL2.dll copy %SDL2BinDir%\SDL2.dll %OutputPath%
if not exist %OutputPath%\SDL2_mixer.dll copy %SDL2MixerBinDir%\SDL2_mixer.dll %OutputPath%

if %BuildMode%==Release rc -nologo -i %ResourcePath% %ResourceFile%

call timer.bat "cl %IncludeDirs% %Defines% %CompilerFlags% %CompilerWarnings% -Fe%OutputExecutable% %InputSource% -link %LinkerFlags% %LinkerWarnings% %LibraryDirs% %Libraries% %InputResource%"

del *.obj

if %BuildMode%==Release (
    if exist %OutputPath%\Resources\ (
        rmdir /s /q %OutputPath%\Resources\
    )
    xcopy assets %OutputPath%\Resources\ /E
)

endlocal
