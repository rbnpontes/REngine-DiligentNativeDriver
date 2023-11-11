#!/bin/bash

echo Generating Project

cmake -S . -B ./build-linux -G "Unix Makefiles" -DCMAKE_BUILD_TYPE="Debug"

echo Building Project

cmake --build build-linux