//
// Created by erhan güven on 16.09.2023.
//

#include "MainMenuScene.h"

namespace Game
{
    void MainMenuScene::start()
    {
        Scene::start();
        auto client = Engine::Client::getInstance();
        onChangeSyncTest = [&](int newVal)
        {
            syncTest = newVal;
        };

        auto ground = spawn<Engine::ImageObject>(1);
        ground->getTransform()->setPositionY(-.5f);
        ground->getTransform()->setScaleY(.5f);
        ground->getTransform()->hasCollider = true;

        auto ground2 = spawn<Engine::ImageObject>(1);
        ground2->getTransform()->setPositionX(-.5f);
        ground2->getTransform()->setScaleX(.75f);
        ground2->getTransform()->setScaleY(.5f);
        ground2->getTransform()->hasCollider = true;

        auto func = [&](const SyncVarTypeVariant& val, int connId)
        {
            spawn<Player>(connId);
        };
        client->getCommandController()->commands["RPC_OnConnect"].emplace_back(func);

        client->getSyncVarHandler()->registerSyncVar(onChangeSyncTest);
    }

    void MainMenuScene::update(float dt)
    {
        Scene::update(dt);
    }
} // Game