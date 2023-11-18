//
// Created by erhan güven on 16.09.2023.
//

#include "MainMenuScene.h"
#include "Input/InputHandler.h"

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

        auto bg = spawn<Engine::ImageObject>("/Users/erhanguven/CLionProjects/growtopia_clone/growtopia_clone/Resources/backgrounds/bg-0.png",0);
        bg->getTransform()->setScaleX(16.0f);
        bg->getTransform()->setScaleY(8.42f);
        bg->getTransform()->setPositionY(.3f);

        for(float x = 0; x < 2.0f; x+=.125f)
        {
            for (float y = 0; y < .165f*4; y+=.165f)
            {
                auto ground = spawn<Engine::ImageObject>("/Users/erhanguven/CLionProjects/growtopia_clone/growtopia_clone/Resources/dirt_mid.png",1);
                ground->getTransform()->setPositionX(x-.9375f);
                ground->getTransform()->setPositionY(y-1.0f+.165f/2.0f);
                ground->getTransform()->hasCollider = true;
                blocks[(int)(x*8)][(int)(y/.165f)]=ground;
            }
        }

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
        int mouseX =(int)Engine::InputHandler::mousePosition.x;
        float mouseY = (Engine::InputHandler::mousePosition.y - .25f*0);
        int block_i = mouseX/50;
        int block_j = (int)(-mouseY/50.0f + 12);

        bool blockExists = block_i >= 0 && block_i <= 15 && block_j >= 0 && block_j <= 3;

        if(blockExists && blocks[block_i][block_j] != nullptr && Engine::InputHandler::onPressMouseButton(GLFW_MOUSE_BUTTON_LEFT))
        {
            blocks[block_i][block_j]->isDead = true;
            blocks[block_i][block_j] = nullptr;
            printf("Deleting: %d,%d\n",block_i,block_j);
        }

    }
} // Game