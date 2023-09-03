#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#include "GLFW/glfw3native.h"
#include "engine/Rendering/Renderer.h"
#include "engine/Window/Window.h"
#include "engine/Rendering/ShaderManager.h"

int main()
{
    Engine::Window::InitializeResolutions();
    auto wdw = Engine::Window(Engine::Resolutions::_800x600, "Growtopia Clone");
    auto* window = wdw.getGlfwWindow();

    Engine::Renderer renderer;

    Engine::ShaderManager shaderManager;
    shaderManager.initShaders({Engine::EngineShaderPrograms::Default});

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xffffffff, 0.0f, 0);

        renderer.render();
        //bgfx::submit(0, Engine::ShaderManager::getInstance()->getProgram(Engine::EngineShaderPrograms::Default));
        //bgfx::frame();

        if(glfwGetKey(window, GLFW_KEY_ESCAPE))
            break;

        if(glfwGetKey(window, GLFW_KEY_1)) // h must be 366
            wdw.setSize(800,600);
    }

    // Cleanup
    bgfx::destroy(shaderManager.getProgram(Engine::EngineShaderPrograms::Default));
    bgfx::shutdown();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
