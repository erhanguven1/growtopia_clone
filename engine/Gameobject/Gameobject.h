//
// Created by erhan güven on 3.09.2023.
//

#ifndef GROWTOPIA_CLONE_GAMEOBJECT_H
#define GROWTOPIA_CLONE_GAMEOBJECT_H

#include <Common/EngineMacros.h>
#include "Rendering/Renderer.h"
#include "Transform.h"

namespace Engine
{
    class Gameobject
    {
        friend class Scene;
        CREATE_ABSTRACT_START_UPDATE;

        Gameobject() = delete;
        explicit Gameobject(uint layerIndex);

        virtual ~Gameobject()
        {
            printf("calling destructor");

        }

        inline void addRenderer()
        {
            assert(renderer == nullptr);
            renderer = std::make_unique<Engine::Renderer>();
        }
        inline void addRenderer(const char* imagePath, bool isUi = false)
        {
            assert(renderer == nullptr);
            renderer = std::make_unique<Engine::Renderer>(imagePath, isUi);
        }

        inline void applyPhysics(float dt)
        {
            transform->applyPhysics(dt);
        }

        inline void tryRender(float dt)
        {
            transform->update(dt, renderer != nullptr);

            if(renderer)
                renderer->render();
        }

        inline Engine::Renderer* getRenderer()
        {
            assert(renderer != nullptr);
            return renderer.get();
        }

        inline Engine::Transform* getTransform()
        {
            assert(transform != nullptr);
            return transform.get();
        }

        inline uint getLayer() const { return layer; }
        bool isDead = false;



    private:
        std::unique_ptr<Engine::Renderer> renderer;
        std::unique_ptr<Engine::Transform> transform;
        uint layer = 1;
        bool initialized = false;
    };
} // Engine

#endif //GROWTOPIA_CLONE_GAMEOBJECT_H