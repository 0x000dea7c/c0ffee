#!/bin/bash

BUILD_TYPE=$(echo "$1" | tr '[:upper:]' '[:lower:]')

if [ "$BUILD_TYPE" != "debug" ] && [ "$BUILD_TYPE" != "release" ]; then
    echo "Error: the first argument must be 'debug' or 'release'"
    exit 1
fi

CMAKE_BUILD_TYPE=""
if [ "$BUILD_TYPE" == "debug" ]; then
    CMAKE_BUILD_TYPE="Debug"
elif [ "$BUILD_TYPE" == "release" ]; then
    CMAKE_BUILD_TYPE="Release"
fi

cmake -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE -S . -B build && cmake --build build
