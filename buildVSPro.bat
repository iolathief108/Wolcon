@ECHO off
REM G- generator name S- source dir B- build dir
cmake -G "Visual Studio 16 2019" -S "." -B ".\VS-cmake-build-debug"
pause