//
// Created by erhan güven on 3.09.2023.
//

#ifndef GROWTOPIA_CLONE_GAMEOBJECT_H
#define GROWTOPIA_CLONE_GAMEOBJECT_H

#include <Common/EngineMacros.h>
#include "Rendering/Renderer.h"

namespace Engine
{
    class Gameobject
    {
        friend class Scene;
        CREATE_ABSTRACT_START_UPDATE;

        Gameobject() = delete;
        Gameobject(uint layerIndex);

        inline void addRenderer()
        {
            assert(renderer == nullptr);
            renderer = std::make_unique<Engine::Renderer>();
        }

        inline Engine::Renderer getRenderer()
        {
            assert(renderer != nullptr);
            return *renderer;
        }

        inline uint getLayer() const { return layer; }

    private:
        std::unique_ptr<Engine::Renderer> renderer;
        uint layer = 1;
        bool initialized = false;
    };
} // Engine

#endif //GROWTOPIA_CLONE_GAMEOBJECT_H