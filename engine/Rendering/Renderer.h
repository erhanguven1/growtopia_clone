//
// Created by erhan güven on 30.08.2023.
//

#ifndef GROWTOPIA_CLONE_RENDERER_H
#define GROWTOPIA_CLONE_RENDERER_H

#include "Mesh.h"
#include <iostream>

namespace Engine
{
    class Renderer
    {
    public:
        Renderer();
        Renderer(const char* imagePath);
        void render();
    private:
        std::unique_ptr<Mesh> mesh;
    };

} // Engine

#endif //GROWTOPIA_CLONE_RENDERER_H
