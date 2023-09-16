//
// Created by erhan güven on 3.09.2023.
//

#ifndef GROWTOPIA_CLONE_SCENE_H
#define GROWTOPIA_CLONE_SCENE_H

#include <Common/EngineMacros.h>
#include <map>
#include <Gameobject/Gameobject.h>

namespace Engine
{
    class Scene
    {
        friend class SceneManager;
    public:
        Scene() = delete;
        explicit Scene(const unsigned int sceneId) : m_SceneId(sceneId) {}

        CREATE_VIRTUAL_START_UPDATE;

        template<class T, typename ... Args>
        void spawn(Args&&... args)
        {
            T* gameObject = new T(std::move(args...));
            newlyCreatedGameObjects.push_back(gameObject);
        }
    private:
        const unsigned int m_SceneId;
        std::map<unsigned int, std::vector<Engine::Gameobject*>> gameObjects;
        std::vector<Engine::Gameobject*> newlyCreatedGameObjects;
        void clear();
    };

} // Engine

#endif //GROWTOPIA_CLONE_SCENE_H
