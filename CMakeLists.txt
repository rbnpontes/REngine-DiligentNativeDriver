cmake_minimum_required (VERSION 3.6)

project(REngine-DiligentNativeDriver CXX)
add_subdirectory(DiligentCore)

file (GLOB SOURCE_FILES src/*.cpp src/*.h)

if(EMSCRIPTEN)
    add_executable(REngine-DiligentNativeDriver ${SOURCE_FILES})
else()
    add_library(REngine-DiligentNativeDriver SHARED ${SOURCE_FILES})
endif()

target_compile_options(REngine-DiligentNativeDriver PRIVATE -DUNICODE -DENGINE_DLL)

set (LIBS 
    Diligent-GraphicsEngineOpenGL-static
    Diligent-GraphicsEngineVk-static)

if (EMSCRIPTEN)
    file(COPY ${CMAKE_SOURCE_DIR}/src/rengine.js DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    list(REMOVE_ITEM LIBS Diligent-GraphicsEngineVk-static)
    add_custom_command(
        TARGET REngine-DiligentNativeDriver
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Bundle Engine Script..."
        COMMAND npx browserify --debug rengine.js > librengine.js
        COMMAND ${CMAKE_COMMAND} -E echo "Finish Script Bundle."
    )
endif()

if (WIN32)
    list(APPEND LIBS Diligent-GraphicsEngineD3D11-static)
    list(APPEND LIBS Diligent-GraphicsEngineD3D12-static)
endif()

if (EMSCRIPTEN)
    target_link_options(
        REngine-DiligentNativeDriver
        PRIVATE
        -sWASM=1
        -sMODULARIZE
        -sALLOW_TABLE_GROWTH=1
        -sNO_EXIT_RUNTIME=1
        -sEXPORTED_RUNTIME_METHODS=[getValue,setValue,stringToUTF8,UTF8ToString,addFunction]
    )
endif()

target_link_libraries(REngine-DiligentNativeDriver
PRIVATE
    ${LIBS}
)