#!/bin/bash
clear;

# check build dir
if [ ! -d build ] 
then 
    mkdir build 
fi

# copy assets files to output dir
cp -R "./resource"  "./build/bin/"

# generate cmake files
cmake -O . -B ./build -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=ON

# compile project
cd build && cmake --build . 

echo compilation finished!