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
        auto client = Engine::Client::getInstance();
        onChangeSyncTest = [&](int newVal)
        {
            syncTest = newVal;
        };
        client->syncVarHandler.registerSyncVar(onChangeSyncTest);
        client->callCommand("CMD_MoveTo", glm::vec2(1,0));

        client->commandController.commands["RPC_UpdatePosition"] = [&](const SyncVarTypeVariant& val, bool isMine)
        {
            RPC_UpdatePosition(val, isMine);
        };

        //client->sendPlayerData(pData);
    }

    void MainMenuScene::update(float dt)
    {
        Scene::update(dt);
        auto postoadd = glm::vec2(0.0001f,0.0f);
        auto client = Engine::Client::getInstance();
        client->callCommand("CMD_MoveTo", glm::vec2(1,0));
        //imageObject->getTransform()->addToPosition(postoadd);
    }

    void MainMenuScene::RPC_UpdatePosition(const SyncVarTypeVariant& val, bool isMine)
    {
        //TODO: A GameObject should get
        glm::vec2 pos = std::get<glm::vec2>(val)*.0001f;
        imageObject->getTransform()->addToPosition(pos);
    }
} // Game