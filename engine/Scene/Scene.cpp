//
// Created by erhan güven on 3.09.2023.
//

#include "Scene.h"

namespace Engine
{

    void Scene::start()
    {
        for (const auto& pair : gameObjects)
        {
            for(auto* gameObject : pair.second)
            {
                gameObject->start();
            }
        }
    }

    void Scene::update(float dt)
    {
        if(!newlyCreatedGameObjects.empty())
        {
            for(const auto& gameObject : newlyCreatedGameObjects)
            {
                gameObject->start();
                gameObjects[gameObject->layer].push_back(gameObject);
            }

            newlyCreatedGameObjects.clear();
        }

        for (const auto& pair : gameObjects)
        {
            for(auto* gameObject : pair.second)
            {
                gameObject->update(dt);
                gameObject->getRenderer().render();
            }
        }
    }

    void Scene::clear()
    {

    }

} // Engine