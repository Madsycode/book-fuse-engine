cls
@echo off

if not exist build mkdir build

rem copy assets & .dll files to output dir
robocopy "./resource"  "./build/bin/Debug/" /e
copy "./windows/bin/**.dll"  "./build/bin/Debug/" /i /e

rem generate cmake files
cmake -O . -B ./build -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=ON

rem compile project
cd build && cmake --build . 

echo compilation finished!
