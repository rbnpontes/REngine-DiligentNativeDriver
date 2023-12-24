@echo off
echo Generating Project

cmake -S . -B ./build-windows

echo Building Project

cmake --build build-windows