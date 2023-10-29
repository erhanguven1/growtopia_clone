#include "bgfx/bgfx.h"
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#include "Gameobject/ImageObject.h"
#include "Window/Window.h"
#include "Rendering/ShaderManager.h"
#include "Scene/SceneManager.h"
#include "Networking/Client.h"

class EngineStarter
{
public:
    void initializeGame(Engine::Resolutions::ResolutionInfo& resolutionInfo, const char* title)
    {
        Engine::Window::InitializeResolutions();
        window = new Engine::Window(resolutionInfo, title);

        shaderManager.initShaders({Engine::EngineShaderPrograms::Default});
    }

    void loop()
    {
        glfwWindow = window->getGlfwWindow();

        float fps = 30.0f;
        float deltaTime;
        std::chrono::steady_clock::time_point lastUpdate;

        while (!glfwWindowShouldClose(glfwWindow))
        {
            auto now = std::chrono::steady_clock::now();
            deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - lastUpdate).count() / 1000000.0f;

            if(deltaTime > 1 / fps)
            {
                lastUpdate = now;
            }

            glfwPollEvents();

            bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x3c3c3cff, 0.0f, 0);

            Engine::SceneManager::getCurrentScene()->update(deltaTime);

            bgfx::frame();

            if(glfwGetKey(glfwWindow, GLFW_KEY_ESCAPE))
                break;

            if(glfwGetKey(glfwWindow, GLFW_KEY_1))
                window->setSize(800,600);
        }
    }

    void exit()
    {
        // Cleanup bgfx
        delete shaderManager.getProgram(Engine::EngineShaderPrograms::Default);
        bgfx::shutdown();

        // Cleanup glfw
        glfwDestroyWindow(glfwWindow);
        delete window;
        glfwTerminate();
    }
private:
    Engine::ShaderManager shaderManager;
    Engine::Window* window = nullptr;
    GLFWwindow* glfwWindow = nullptr;
};