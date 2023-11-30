echo Generating Project

cmake -S . -B ./build-android -G "Ninja" -DCMAKE_BUILD_TYPE="Debug" -DANDROID_STL=c++_static -DCMAKE_TOOLCHAIN_FILE=%NDK_ROOT%/build/cmake/android.toolchain.cmake -DANDROID_ABI=armeabi-v7a -DANDROID_NATIVE_API_LEVEL=21

echo Building Project

cmake --build build-android