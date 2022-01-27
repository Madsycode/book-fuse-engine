#!/bin/bash
clear;

if [ ! -d build ]
then
    mkdir build
fi

cd build
cmake -O .. -B . -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=ON
cmake --build . 

echo done!