//
// Created by erhan güven on 30.08.2023.
//

#ifndef GROWTOPIA_CLONE_RENDERER_H
#define GROWTOPIA_CLONE_RENDERER_H

#include "Mesh.h"
#include <iostream>
#include <glm/vec4.hpp>

namespace Engine
{
    class Renderer
    {
    public:
        Renderer();
        Renderer(const char* imagePath, bool isUi = false);
        void render();
        inline void setColor(glm::vec4& color)
        {
            m_Color = color;
        }
        void setTexture(const char* imagePath);
    private:
        std::unique_ptr<Mesh> mesh;
        glm::vec4 m_Color = {1.0f,1.0f,1.0f,1.0f};
    };

} // Engine

#endif //GROWTOPIA_CLONE_RENDERER_H
