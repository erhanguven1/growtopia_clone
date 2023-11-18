//
// Created by erhan güven on 30.08.2023.
//

#include "Window.h"
#include <iostream>

namespace Engine
{

    Window::Window(int width, int height, const char *title)
    {
        // Initialize GLFW
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW!");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        // Create a GLFW window
        glfwWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!glfwWindow) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW Window!");
        }

        //glfwSetWindowPos(glfwWindow,0,height/2);

        // Initialize BGFX with the GLFW window
        bgfx::PlatformData pd;
        pd.nwh = glfwGetCocoaWindow(glfwWindow);

        // Initialize BGFX
        bgfx::Init bgfxInit;
        bgfxInit.platformData = pd;
        bgfxInit.type = bgfx::RendererType::OpenGL; // Change renderer type as needed

        bgfx::renderFrame();

        if(!bgfx::init(bgfxInit))
            throw std::runtime_error("Failed to initialize BGFX!");

        bgfx::setViewClear(0, BGFX_CLEAR_COLOR, 0x000000FF);
        bgfx::setViewRect(0, 0, 120, static_cast<std::uint16_t>(width), static_cast<std::uint16_t>(height));

        setSize(width, height);

        setWindowCallbacks();

        instance = this;

    }

    void Window::setWindowCallbacks()
    {
        glfwSetWindowUserPointer(glfwWindow, this);
    }

    void Window::setWidth(const int &w)
    {
        setSize(w, height);
    }

    void Window::setHeight(const int &h)
    {
        setSize(width, h);
    }

    void Window::setSize(const int &w, const int &h)
    {
        width = w;
        height = h;
        glfwSetWindowSize(glfwWindow,width,height);

        int xPos, yPos;
        glfwGetWindowPos(glfwWindow,&xPos,&yPos);

        printf("X = %d, Y = %d\n", xPos, yPos);

        float y = 320 - (height - 400);

        printf("H = %f\n", y);


        bgfx::setViewRect(0,-width/2,y,width,height);
        windowSize.x = width;
        windowSize.y = height;
        //bgfx::setViewRect(0, 0, 120-height+600, static_cast<std::uint16_t>(width), static_cast<std::uint16_t>(height));

    }
} // Engine