cmake_minimum_required(VERSION 3.7)
project(growtopia_clone)

set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES engine/EngineStarter.hpp
        logo.h
        engine/Rendering/Mesh.cpp
        engine/Rendering/Mesh.h
        engine/Rendering/Renderer.cpp
        engine/Rendering/Renderer.h
        engine/Window/Window.cpp
        engine/Window/Window.h
        engine/Rendering/ShaderManager.h
        engine/Gameobject/Gameobject.cpp
        engine/Gameobject/Gameobject.h
        engine/Scene/Scene.cpp
        engine/Scene/Scene.h
        engine/Common/EngineMacros.h
        engine/Component/Component.cpp
        engine/Component/Component.h
        engine/Gameobject/ImageObject.cpp
        engine/Gameobject/ImageObject.h
        engine/Scene/SceneManager.cpp
        engine/Scene/SceneManager.h
        growtopia_clone/Scenes/MainMenuScene.cpp
        growtopia_clone/Scenes/MainMenuScene.h
        growtopia_clone/entrypoint.cpp
        engine/Gameobject/Transform.cpp
        engine/Gameobject/Transform.h
        engine/Networking/Client.cpp
        engine/Networking/Client.h
)

set(NETWORKING_SOURCE_FILES
        engine/Networking/NetworkStructs.h)

set(SERVER_SOURCE_FILES server/Server.cpp)

set(BGFX_BUILD_TOOLS ON CACHE INTERNAL "" FORCE)

add_subdirectory("thirdparty/bgfx.cmake")
include_directories("thirdparty/bgfx.cmake/bgfx/include")
include_directories("engine")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}")

find_package(ENet REQUIRED)
include_directories(${ENET_INCLUDE_DIRS})
link_libraries(${ENET_LIBRARIES})

add_executable(growtopia_clone ${SOURCE_FILES} ${NETWORKING_SOURCE_FILES})
add_executable(growtopia_clone_second ${SOURCE_FILES} ${NETWORKING_SOURCE_FILES})
add_executable(server_exe ${SERVER_SOURCE_FILES} ${NETWORKING_SOURCE_FILES})

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

target_link_libraries(growtopia_clone bgfx example-common ${OPENGL_LIBRARIES} glfw)
target_link_libraries(growtopia_clone_second bgfx example-common ${OPENGL_LIBRARIES} glfw)

add_custom_target(
        build-shaders
        COMMAND $<TARGET_FILE:shaderc> -f ${CMAKE_CURRENT_SOURCE_DIR}/shaders/vs_default.sc -o ${CMAKE_CURRENT_SOURCE_DIR}/glsl/vs_default.bin --type v -i ${BGFX_DIR}/src --varyingdef ${CMAKE_CURRENT_SOURCE_DIR}/shaders/varying.def.sc
        COMMAND $<TARGET_FILE:shaderc> -f ${CMAKE_CURRENT_SOURCE_DIR}/shaders/fs_default.sc -o ${CMAKE_CURRENT_SOURCE_DIR}/glsl/fs_default.bin --type f -i ${BGFX_DIR}/src --varyingdef ${CMAKE_CURRENT_SOURCE_DIR}/shaders/varying.def.sc
)