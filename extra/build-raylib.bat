#!/bin/bash

set OLD_PATH=%PATH%
set MINGW32=E:\workspaces\contributes\MinGW\MinGW32
set MINGW64=E:\workspaces\contributes\MinGW\MinGW64

for /f %%i in ('cd') do set RDRAWING_FOLDER=%%i

RAYLIB_FOLDER=%RDRAWING_FOLDER%\..\raylib

set TARGET_FOLDER_ROOT="R:"

pushd .

rmdir /S /Q "%TEMP%/build-rdrawing"
mkdir "%TEMP%/build-rdrawing"
cd /d "%TEMP%/build-rdrawing"

set PATH=%MINGW32%\bin;%OLD_PATH%

cmake "%RAYLIB_FOLDER%" -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
mingw32-make -j8

set TARGET_FOLDER=%TARGET_FOLDER_ROOT%\MinGW32

rmdir /S /Q "%TARGET_FOLDER%"
mkdir "%TARGET_FOLDER%" 
mkdir "%TARGET_FOLDER%\i686-w64-mingw32" 
mkdir "%TARGET_FOLDER%\i686-w64-mingw32\lib" 
xcopy /Y /I "src\include" "%TARGET_FOLDER%\i686-w64-mingw32\include" 
copy /Y "src\libraylib.a" "%TARGET_FOLDER%\i686-w64-mingw32\lib" 

popd

pushd .

rmdir /S /Q "%TEMP%/build-rdrawing"
mkdir "%TEMP%/build-rdrawing"
cd /d "%TEMP%/build-rdrawing"

set PATH=%MINGW64%\bin;%OLD_PATH%

echo %PATH%

cmake "%RAYLIB_FOLDER%" -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
mingw32-make -j8

set TARGET_FOLDER=%TARGET_FOLDER_ROOT%\MinGW64

rmdir /S /Q "%TARGET_FOLDER%"
mkdir "%TARGET_FOLDER%" 
mkdir "%TARGET_FOLDER%\x86_64-w64-mingw32" 
mkdir "%TARGET_FOLDER%\x86_64-w64-mingw32\lib" 
xcopy /Y /I "src\include" "%TARGET_FOLDER%\x86_64-w64-mingw32\include" 
copy /Y "src\libraylib.a" "%TARGET_FOLDER%\x86_64-w64-mingw32\lib" 

popd
