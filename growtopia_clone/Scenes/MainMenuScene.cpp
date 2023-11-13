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
        ground->getTransform()->setScaleX(2.0f);
        ground->getTransform()->setScaleY(.5f);
        ground->getTransform()->hasCollider = true;

        auto ground2 = spawn<Engine::ImageObject>(1);
        ground2->getTransform()->setPositionX(-.5f);
        ground2->getTransform()->setScaleX(.75f);
        ground2->getTransform()->setScaleY(.5f);
        ground2->getTransform()->hasCollider = true;

        auto func = [&](const SyncVarTypeVariant& val, int connId)
        {
            spawn<Player>(connId, true);
        };
        client->getCommandController()->commands["RPC_OnConnect"].emplace_back(func);

        auto onFetchOtherPlayer = [&](const SyncVarTypeVariant& val, int connId)
        {
            auto secondId = (uint)std::get<int>(val);
            spawn<Player>(secondId, false);
        };
        client->getCommandController()->commands["RPC_OnFetchOtherPlayer"].emplace_back(onFetchOtherPlayer);

        client->getSyncVarHandler()->registerSyncVar(onChangeSyncTest);
    }

    void MainMenuScene::update(float dt)
    {
        Scene::update(dt);
    }
} // Game