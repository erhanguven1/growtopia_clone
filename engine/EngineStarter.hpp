#include "bgfx/bgfx.h"
#include "bx/math.h"
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#include "Gameobject/ImageObject.h"
#include "Window/Window.h"
#include "Rendering/ShaderManager.h"
#include "Scene/SceneManager.h"
#include "Networking/Client.h"
#include "Input/InputHandler.h"
#include "Rendering/Camera.h"

class EngineStarter
{
public:
    void initializeGame(Engine::Resolutions::ResolutionInfo& resolutionInfo, const char* title)
    {
        Engine::Window::InitializeResolutions();
        window = new Engine::Window(resolutionInfo, title);
        glfwWindow = window->getGlfwWindow();

        shaderManager.initShaders({Engine::EngineShaderPrograms::Default});

        Engine::InputHandler* inputHandler = new Engine::InputHandler();

        glfwSetKeyCallback(glfwWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            Engine::InputHandler* instance = static_cast<Engine::InputHandler*>(glfwGetWindowUserPointer(window));
            instance->keyEvent(key, action);
        });

        glfwSetCursorPosCallback(glfwWindow, [](GLFWwindow* window, double xPos, double yPos)
        {
            Engine::InputHandler* instance = static_cast<Engine::InputHandler*>(glfwGetWindowUserPointer(window));
            instance->cursorPosEvent(xPos, yPos);
        });

        glfwSetMouseButtonCallback(glfwWindow, [](GLFWwindow* window, int button, int action, int mods)
        {
            Engine::InputHandler* instance = static_cast<Engine::InputHandler*>(glfwGetWindowUserPointer(window));
            instance->mouseButtonEvent(button, action, mods);
        });


        glfwSetWindowUserPointer(glfwWindow, &inputHandler);
    }

    void loop()
    {
        float fps = 60.0f;
        float deltaTime;
        std::chrono::steady_clock::time_point lastUpdate;

        auto camera = Engine::Camera::getDefault();

        while (!glfwWindowShouldClose(glfwWindow))
        {
            auto now = std::chrono::steady_clock::now();
            deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - lastUpdate).count() / 1000000.0f;

            glfwPollEvents();

            if(deltaTime < 1 / fps)
            {
                continue;
            }
            lastUpdate = now;

            bgfx::setViewClear(0, BGFX_CLEAR_COLOR, 0x00ffffff, 0.0f, 0);

            float proj[16];
            float width = 800.0f;
            float height = 600.0f;

            glm::vec2 camPos = camera->getPosition();
            const float zoom = camera->getZoom();

            bx::mtxOrtho(proj, (-width/2)/zoom + camPos.x, (width/2)/zoom + camPos.x, (-height/2)/zoom + camPos.y,(height/2)/zoom + camPos.y, 0.0f, 1000.0f,0.0f, bgfx::getCaps()->homogeneousDepth);
            bgfx::setViewTransform(0, nullptr, proj);

            if(deltaTime < 100.0f)
                Engine::SceneManager::getCurrentScene()->update(deltaTime);

            bgfx::frame();

            if(glfwGetKey(glfwWindow, GLFW_KEY_ESCAPE))
                break;

            if(glfwGetKey(glfwWindow, GLFW_KEY_1))
                window->setSize(800,600);
        }
        Engine::Client::getInstance()->disconnectRequested = true;
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