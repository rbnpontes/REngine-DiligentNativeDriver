cmake_minimum_required (VERSION 3.6)

project(REngine-DiligentNativeDriver CXX)

add_subdirectory(DiligentCore)

file (GLOB SOURCE_FILES src/*.cpp src/*.h)

add_library(REngine-DiligentNativeDriver SHARED ${SOURCE_FILES})
target_compile_options(REngine-DiligentNativeDriver PRIVATE -DUNICODE -DENGINE_DLL)

set (LIBS 
    Diligent-GraphicsEngineOpenGL-static
    Diligent-GraphicsEngineVk-static)

if (WIN32)
    list(APPEND LIBS Diligent-GraphicsEngineD3D11-static)
    list(APPEND LIBS Diligent-GraphicsEngineD3D12-static)
endif()

target_link_libraries(REngine-DiligentNativeDriver
PRIVATE
    ${LIBS}
)