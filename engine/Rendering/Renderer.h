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
        void render();
    private:
        Mesh* mesh = nullptr;
    };

} // Engine

#endif //GROWTOPIA_CLONE_RENDERER_H
