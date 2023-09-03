cmake_minimum_required(VERSION 3.7)
project(growtopia_clone)

set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES main.cpp
        logo.h
        engine/Rendering/Mesh.cpp
        engine/Rendering/Mesh.h
        engine/Rendering/Renderer.cpp
        engine/Rendering/Renderer.h
        engine/Window/Window.cpp
        engine/Window/Window.h
        engine/Rendering/ShaderManager.h
)
set(BGFX_BUILD_TOOLS ON CACHE INTERNAL "" FORCE)

add_subdirectory("thirdparty/bgfx.cmake")
include_directories("thirdparty/bgfx.cmake/bgfx/include")
add_executable(growtopia_clone ${SOURCE_FILES} )

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

target_link_libraries(growtopia_clone bgfx example-common ${OPENGL_LIBRARIES} glfw)

add_custom_target(
        build-shaders
        COMMAND $<TARGET_FILE:shaderc> -f ${CMAKE_CURRENT_SOURCE_DIR}/shaders/vs_default.sc -o ${CMAKE_CURRENT_SOURCE_DIR}/glsl/vs_default.bin --type v -i ${BGFX_DIR}/src --varyingdef ${CMAKE_CURRENT_SOURCE_DIR}/shaders/varying.def.sc
        COMMAND $<TARGET_FILE:shaderc> -f ${CMAKE_CURRENT_SOURCE_DIR}/shaders/fs_default.sc -o ${CMAKE_CURRENT_SOURCE_DIR}/glsl/fs_default.bin --type f -i ${BGFX_DIR}/src --varyingdef ${CMAKE_CURRENT_SOURCE_DIR}/shaders/varying.def.sc
)