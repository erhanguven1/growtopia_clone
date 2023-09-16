//
// Created by erhan güven on 16.09.2023.
//

#include "SceneManager.h"

namespace Engine
{
    void SceneManager::setCurrentScene(Scene *&scene)
    {
        if(currentScene != nullptr)
        {
            currentScene->clear();
        }
        currentScene = scene;
    }
} // Engine