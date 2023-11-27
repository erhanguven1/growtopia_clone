//
// Created by erhan güven on 16.09.2023.
//

#include "MainMenuScene.h"
#include "Input/InputHandler.h"
#include "../World/WorldLoader.h"
#include "Rendering/Camera.h"

namespace Game
{
    void MainMenuScene::start()
    {
        Scene::start();
        auto client = Engine::Client::getInstance();

        Engine::Camera::getDefault()->setZoom(1.5f);

        auto bg = spawn<Engine::ImageObject>("/Users/erhanguven/CLionProjects/growtopia_clone/growtopia_clone/Resources/backgrounds/bg-0.png",0);
        bg->getTransform()->setScaleX(800.0f);
        bg->getTransform()->setScaleY(419.735259f);
        bg->getTransform()->setPositionY(1.0f/3.0f);

        auto bg2 = spawn<Engine::ImageObject>("/Users/erhanguven/CLionProjects/growtopia_clone/growtopia_clone/Resources/backgrounds/bg-0.png",0);
        bg2->getTransform()->setScaleX(800.0f);
        bg2->getTransform()->setScaleY(419.735259f);
        bg2->getTransform()->setPositionX(800.0f);
        bg2->getTransform()->setPositionY(1.0f/3.0f);

        auto bg3 = spawn<Engine::ImageObject>("/Users/erhanguven/CLionProjects/growtopia_clone/growtopia_clone/Resources/backgrounds/bg-0.png",0);
        bg3->getTransform()->setScaleX(800.0f);
        bg3->getTransform()->setScaleY(419.735259f);
        bg3->getTransform()->setPositionX(-800.0f);
        bg3->getTransform()->setPositionY(1.0f/3.0f);

        for(float x = -400.0f; x < 400.0f; x += 50.0f)
        {
            for (float y = -300.0f; y < -100.0f; y += 50.0f)
            {
                int i = int((x+400)/50);
                int j = int((y+300)/50);

                glm::ivec2 pos = {x+25.0f,y+25.0f};
                auto ground = new Tile(i,j,BlockType::Dirt, pos);

                blocks[i][j] = ground;
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

        World world = World();
        WorldLoader::saveWorld("world0", &world);
        printf("ok");
    }

    void MainMenuScene::update(float dt)
    {
        Scene::update(dt);

        float zoom = Engine::Camera::getDefault()->getZoom();

        int mouseX =(int)((Engine::InputHandler::mousePosition.x - 400.0f) / zoom + Engine::Camera::getDefault()->getPosition().x);
        int mouseY = (int)((Engine::InputHandler::mousePosition.y - 300.0f) / zoom - Engine::Camera::getDefault()->getPosition().y);
        int block_i = (mouseX+400)/50;
        int block_j = (int)(float(-mouseY+250)/50.0f + 1);

        bool blockExists = block_i >= 0 && block_i <= 15 && block_j >= 0 && block_j <= 3;

        if(blockExists && blocks[block_i][block_j] != nullptr && Engine::InputHandler::isPressingMouseButton(GLFW_MOUSE_BUTTON_LEFT))
        {

            glm::vec2& playerPos =Player::myPlayer->getCharacterPosition();
            glm::vec2& blockPos =blocks[block_i][block_j]->getBlockPosition();

            printf("%d,%d. Distance to player = %f\n",block_i,block_j,glm::distance(playerPos,blockPos));

            if(glm::distance(playerPos,blockPos) < 120.0f)
            {
                blocks[block_i][block_j]->takeDamage(dt * 100);
            }
        }

        if(Engine::InputHandler::isPressingKey(GLFW_KEY_Z))
        {
            Engine::Camera::getDefault()->changeZoom(.01f);
        }
        if(Engine::InputHandler::isPressingKey(GLFW_KEY_X))
        {
            Engine::Camera::getDefault()->changeZoom(-.01f);
        }

        if(Engine::InputHandler::isPressingKey(GLFW_KEY_RIGHT))
        {
            auto newPos = Engine::Camera::getDefault()->getPosition()+glm::vec2(150.0f*dt,0.0f);
            Engine::Camera::getDefault()->setPosition(newPos);
        }
        if(Engine::InputHandler::isPressingKey(GLFW_KEY_LEFT))
        {
            auto newPos = Engine::Camera::getDefault()->getPosition()-glm::vec2(150.0f*dt,0.0f);
            Engine::Camera::getDefault()->setPosition(newPos);
        }
        if(Engine::InputHandler::isPressingKey(GLFW_KEY_UP))
        {
            auto newPos = Engine::Camera::getDefault()->getPosition()+glm::vec2(0.0f,150.0f*dt);
            Engine::Camera::getDefault()->setPosition(newPos);
        }
        if(Engine::InputHandler::isPressingKey(GLFW_KEY_DOWN))
        {
            auto newPos = Engine::Camera::getDefault()->getPosition()-glm::vec2(0.0f,150.0f*dt);
            Engine::Camera::getDefault()->setPosition(newPos);
        }
        if(Player::myPlayer)
        {
            float posX = Engine::Camera::getDefault()->getPosition().x;
            float posY = Engine::Camera::getDefault()->getPosition().y;

            if(Player::myPlayer->getCharacterPosition().y > -200)
                posY = Player::myPlayer->getCharacterPosition().y;
            if(Player::myPlayer->getCharacterPosition().x > -400 && Player::myPlayer->getCharacterPosition().x < 400)
                posX = Player::myPlayer->getCharacterPosition().x;

            Engine::Camera::getDefault()->setPosition({posX,posY});
        }
    }
} // Game