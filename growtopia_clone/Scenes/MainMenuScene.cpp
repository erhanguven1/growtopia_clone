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