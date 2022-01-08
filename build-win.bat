cls
@echo off
if not exist build mkdir build
cd build

cmake -O .. -B . -D CMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=ON
cmake --build . 

echo compiled successfully!