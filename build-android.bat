@echo off
set abi=armeabi-v7a

echo Building Armeabi-v7a
cmake -S . -B ./build-android-%abi% -G "Ninja" -DANDROID_STL=c++_static -DCMAKE_TOOLCHAIN_FILE=%NDK_ROOT%/build/cmake/android.toolchain.cmake -DANDROID_ABI=%abi% -DANDROID_NATIVE_API_LEVEL=21
cmake --build build-android-%abi% --config Release

set abi=arm64-v8a
echo Building Arm64-V8A
cmake -S . -B ./build-android-%abi% -G "Ninja" -DANDROID_STL=c++_static -DCMAKE_TOOLCHAIN_FILE=%NDK_ROOT%/build/cmake/android.toolchain.cmake -DANDROID_ABI=%abi% -DANDROID_NATIVE_API_LEVEL=21
cmake --build build-android-%abi% --config Release

set abi=x86
echo Generating Project
cmake -S . -B ./build-android-%abi% -G "Ninja" -DANDROID_STL=c++_static -DCMAKE_TOOLCHAIN_FILE=%NDK_ROOT%/build/cmake/android.toolchain.cmake -DANDROID_ABI=%abi% -DANDROID_NATIVE_API_LEVEL=21
cmake --build build-android-%abi% --config Release

set abi=x86_64
echo Generating Project
cmake -S . -B ./build-android-%abi% -G "Ninja" -DANDROID_STL=c++_static -DCMAKE_TOOLCHAIN_FILE=%NDK_ROOT%/build/cmake/android.toolchain.cmake -DANDROID_ABI=%abi% -DANDROID_NATIVE_API_LEVEL=21
cmake --build build-android-%abi% --config Release