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
            for (float y = 0, h = 0; y < .165f*4; y+=.165f, h += 1.0f)
            {
                glm::ivec2 pos = {x*8,h};
                auto ground = new Tile(BlockType::Dirt, pos);
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

        auto onBlockDestroyed = [&](const SyncVarTypeVariant& val, int connId)
        {
            auto blockIndex = (glm::ivec2)std::get<glm::vec2>(val);
            blocks[blockIndex.x][blockIndex.y]->setBlockType(BlockType::Empty);
        };
        client->getCommandController()->commands["RPC_OnBlockDestroyed"].emplace_back(onBlockDestroyed);

        client->getSyncVarHandler()->registerSyncVar(onChangeSyncTest);
    }

    void MainMenuScene::update(float dt)
    {
        Scene::update(dt);
        int mouseX =(int)Engine::InputHandler::mousePosition.x;
        float mouseY = (Engine::InputHandler::mousePosition.y);
        int block_i = mouseX/50;
        int block_j = (int)(-mouseY/50.0f + 12);

        bool blockExists = block_i >= 0 && block_i <= 15 && block_j >= 0 && block_j <= 3;

        if(blockExists && blocks[block_i][block_j] != nullptr && Engine::InputHandler::isPressingMouseButton(GLFW_MOUSE_BUTTON_LEFT))
        {
            glm::vec2& playerPos =Player::myPlayer->getCharacterPosition();
            glm::vec2& blockPos =blocks[block_i][block_j]->getBlockPosition();
            if(glm::distance(playerPos,blockPos) < .333f)
            {
                blocks[block_i][block_j]->takeDamage(dt * 100);
            }
        }

    }
} // Game