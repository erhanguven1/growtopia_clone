//
// Created by erhan güven on 16.09.2023.
//

#include "MainMenuScene.h"

namespace Game
{
    void MainMenuScene::start()
    {
        Scene::start();
        imageObject = spawn<Engine::ImageObject>(1);
        auto imageObject2 = spawn<Engine::ImageObject>(1);
    }

    void MainMenuScene::update(float dt)
    {
        Scene::update(dt);
        auto postoadd = glm::vec2(0.0001f,0.0f);
        imageObject->getTransform()->addToPosition(postoadd);
    }
} // Game