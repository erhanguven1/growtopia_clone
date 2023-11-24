//
// Created by erhan güven on 30.08.2023.
//

#ifndef GROWTOPIA_CLONE_WINDOW_H
#define GROWTOPIA_CLONE_WINDOW_H

#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#include "GLFW/glfw3native.h"
#include <glm/glm.hpp>
#include <unordered_map>
#include <string>

#define SetResVecFromName(name, val) setResolutionValuesFromName(#name, val)

namespace Engine
{
    class Window;

    namespace Resolutions
    {
        struct ResolutionInfo
        {
            friend class Engine::Window;
        private:
            glm::ivec2 resolution = glm::ivec2(100,100);
        };

        inline ResolutionInfo _800x600{};
    }

    class Window
    {
    public:
        static void setResolutionValuesFromName(char* name, Resolutions::ResolutionInfo& resInfo)
        {
            std::string str(name);
            bool fillingWidth = true;
            std::string width;
            std::string height;
            for(auto c : str)
            {
                if(c == '_')
                    continue;
                if(fillingWidth)
                {
                    if(c == 'x')
                        fillingWidth = false;
                    else
                        width += c;

                    continue;
                }
                else
                {
                    height += c;
                    continue;
                }
            }

            resInfo.resolution = glm::vec2(atoi(width.c_str()), atoi(height.c_str()));
        }
        static void InitializeResolutions()
        {
            using namespace Resolutions;
            SetResVecFromName(_800x600,_800x600);
        }
        inline static Window* getInstance(){ return instance; }

        Window(int width, int height, const char* title);
        Window(Resolutions::ResolutionInfo resInfo, const char* title) : Window(resInfo.resolution.x, resInfo.resolution.y, title)
        {

        }
        inline GLFWwindow* getGlfwWindow(){ return glfwWindow; }

        void setWidth(const int& w);
        void setHeight(const int& h);
        void setSize(const int& w, const int& h);
        inline glm::vec2 getSize() const {return {width,height};}
        inline static glm::vec2 windowSize = glm::vec2(800,600);

        void zoom(float zoomAdd);

        float yOffset = 0;
        float zoomLevel = 1.0f;

    private:
        inline static Window* instance;
        GLFWwindow* glfwWindow = nullptr;
        int width, height;

        void setWindowCallbacks();
    };
} // Engine

#endif //GROWTOPIA_CLONE_WINDOW_H
