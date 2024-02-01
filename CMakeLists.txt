cmake_minimum_required (VERSION 3.6)

project(REngine-DiligentNativeDriver CXX)
add_subdirectory(DiligentCore)

file (GLOB SOURCE_FILES src/*.cpp src/*.h)

if(EMSCRIPTEN)
    add_executable(REngine-DiligentNativeDriver ${SOURCE_FILES})
else()
    list(REMOVE_ITEM SOURCE_FILES src/EmscriptenBindings.h)
    add_library(REngine-DiligentNativeDriver SHARED ${SOURCE_FILES})
endif()

target_compile_options(REngine-DiligentNativeDriver PRIVATE -DUNICODE -DENGINE_DLL)

set (LIBS 
    Diligent-GraphicsEngineOpenGL-static
    Diligent-GraphicsEngineVk-static)

if (EMSCRIPTEN)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/output)
    # Emscripten does not support Vulkan, we need to remove then
    list(REMOVE_ITEM LIBS Diligent-GraphicsEngineVk-static)
    # Create JS source symbolic link
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_CURRENT_SOURCE_DIR}/jslib" "${CMAKE_CURRENT_BINARY_DIR}/jslib"
        OUTPUT_QUIET ERROR_QUIET
    )

    # Add browserify build command
    # add_custom_command(
    #     TARGET REngine-DiligentNativeDriver
    #     POST_BUILD
    #     COMMAND ${CMAKE_COMMAND} -E echo "Bundle Engine Script..."
    #     COMMAND npx browserify --debug -r ./REngine-DiligentNativeDriver.js:librengine-module.js > ./output/librengine-module.js
    #     COMMAND npx browserify -x librengine-module.js --debug ./jslib/rengine.js > ./output/librengine.js
    #     COMMAND ${CMAKE_COMMAND} -E echo "Finish Script Bundle."
    #     COMMAND ${CMAKE_COMMAND} -E echo "Copying WASM file"
    #     COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/REngine-DiligentNativeDriver.wasm  ${CMAKE_CURRENT_BINARY_DIR}/output
    #     COMMAND ${CMAKE_COMMAND} -E echo "Finish"
    # )
endif()

if (WIN32)
    list(APPEND LIBS Diligent-GraphicsEngineD3D11-static)
    list(APPEND LIBS Diligent-GraphicsEngineD3D12-static)
endif()

if (EMSCRIPTEN)
    set(CMAKE_BUILD_TYPE Debug)

    target_link_options(
        REngine-DiligentNativeDriver
        PRIVATE
        -g
        -sWASM=1
        -sEXPORT_ES6=1
        -sMODULARIZE
        -sALLOW_TABLE_GROWTH=1
        -sNO_EXIT_RUNTIME=1
        -sEXPORTED_RUNTIME_METHODS=[getValue,setValue,UTF8ToString,allocateUTF8,addFunction]
    )
endif()

target_link_libraries(REngine-DiligentNativeDriver
PRIVATE
    ${LIBS}
)