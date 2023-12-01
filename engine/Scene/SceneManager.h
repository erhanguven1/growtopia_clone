//
// Created by erhan güven on 16.09.2023.
//

#ifndef GROWTOPIA_CLONE_SCENEMANAGER_H
#define GROWTOPIA_CLONE_SCENEMANAGER_H

#include "Scene.h"

namespace Engine
{

    class SceneManager
    {
    public:
        static void addScene(unsigned int id, Scene& sceneToAdd);
        static void setCurrentScene(int sceneId);
        inline static void spawnInCurrentScene()
        {
            assert(currentScene != nullptr);
        }
        inline static Scene* getCurrentScene()
        {
            assert(currentScene != nullptr);
            return currentScene;
        }
    private:
        inline static Scene* currentScene;
        inline static std::map<unsigned int, Scene*> scenes;
    };

} // Engine

#endif //GROWTOPIA_CLONE_SCENEMANAGER_H
