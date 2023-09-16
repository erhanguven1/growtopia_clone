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
        static void setCurrentScene(Scene*& scene);
        inline static void spawnInCurrentScene()
        {
            assert(currentScene != nullptr);
        }
    private:
        static Scene* currentScene;
    };

} // Engine

#endif //GROWTOPIA_CLONE_SCENEMANAGER_H
