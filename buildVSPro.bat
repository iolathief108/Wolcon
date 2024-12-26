@ECHO off
REM G- generator name S- source dir B- build dir
cmake -G "Visual Studio 2022" -S "." -B ".\VS-cmake-build-debug"
pause
