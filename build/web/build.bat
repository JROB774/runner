@echo off
setlocal

call config.bat

if not exist %OutputPath% mkdir %OutputPath%

call timer.bat emcc %InputSource% %IncludeDirs% %Libraries% %Defines% %CompilerFlags% %LinkerFlags% --preload-file %AssetPath% -o %OutputExecutable%.html

:: copy dev\web\index.html %OutputPath%\index.html

endlocal
