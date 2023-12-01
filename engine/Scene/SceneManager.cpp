//
// Created by erhan güven on 16.09.2023.
//

#include "SceneManager.h"

namespace Engine
{
    void SceneManager::addScene(unsigned int id, Scene &sceneToAdd)
    {
        scenes[id] = &sceneToAdd;
    }
    void SceneManager::setCurrentScene(int sceneId)
    {
        assert(scenes[sceneId]);
        printf("Loaded scene: %u",sceneId);
        if(currentScene != nullptr)
        {
            currentScene->clear();
        }
        currentScene = scenes[sceneId];
        currentScene->start();
    }
} // Engine