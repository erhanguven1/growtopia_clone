#include "bgfx/bgfx.h"
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#include "Gameobject/ImageObject.h"
#include "Window/Window.h"
#include "Rendering/ShaderManager.h"

int main()
{
    Engine::Window::InitializeResolutions();
    auto wdw = Engine::Window(Engine::Resolutions::_800x600, "Growtopia Clone");
    auto* window = wdw.getGlfwWindow();

    Engine::ImageObject imageObject = Engine::ImageObject();
    imageObject.addRenderer();

    Engine::ShaderManager shaderManager;
    shaderManager.initShaders({Engine::EngineShaderPrograms::Default});

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xffffffff, 0.0f, 0);

        imageObject.getRenderer().render();

        if(glfwGetKey(window, GLFW_KEY_ESCAPE))
            break;

        if(glfwGetKey(window, GLFW_KEY_1))
            wdw.setSize(800,600);
    }

    // Cleanup
    bgfx::destroy(shaderManager.getProgram(Engine::EngineShaderPrograms::Default));
    bgfx::shutdown();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
