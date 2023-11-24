//
// Created by erhan güven on 16.09.2023.
//

#include "MainMenuScene.h"
#include "Input/InputHandler.h"
#include "Window/Window.h"

namespace Game
{
    void MainMenuScene::start()
    {
        Scene::start();
        auto client = Engine::Client::getInstance();

        auto bg = spawn<Engine::ImageObject>("/Users/erhanguven/CLionProjects/growtopia_clone/growtopia_clone/Resources/backgrounds/bg-0.png",0);
        bg->getTransform()->setScaleX(800.0f);
        bg->getTransform()->setScaleY(600.0f);
        bg->getTransform()->setPositionY(1.0f/3.0f);
        bg->getTransform()->debugBreak = true;

        for(float x = -400.0f; x < 400.0f; x += 50.0f)
        {
            for (float y = -300.0f; y < -100.0f; y += 50.0f)
            {
                glm::ivec2 pos = {x+25.0f,y+25.0f};
                auto ground = new Tile(BlockType::Dirt, pos);

                int i = int((x+400)/25);
                int j = int((y+300)/50);

                blocks[i][j]=ground;
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
            if(connId == Engine::Client::getInstance()->getConnectionId())
                return;

            auto blockIndex = (glm::ivec2)std::get<glm::vec2>(val);
            if(blocks[blockIndex.x][blockIndex.y]->getBlockType() == BlockType::Dirt)
                blocks[blockIndex.x][blockIndex.y]->setBlockType(BlockType::CaveBackground, true);
            else
                blocks[blockIndex.x][blockIndex.y]->setBlockType(BlockType::Empty, true);
        };
        client->getCommandController()->commands["RPC_OnBlockDestroyed"].emplace_back(onBlockDestroyed);
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

        if(Engine::InputHandler::isPressingKey(GLFW_KEY_Z))
        {
            Engine::Window::getInstance()->zoom(.1f);
        }
        if(Engine::InputHandler::isPressingKey(GLFW_KEY_X))
        {
            Engine::Window::getInstance()->zoom(-.1f);
        }
    }
} // Game