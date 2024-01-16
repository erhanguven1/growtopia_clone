//
// Created by erhan güven on 1.12.2023.
//

#include "GameScene.h"
#include "Gameobject/Button.h"
#include "Networking/Client.h"
#include "Rendering/Camera.h"
#include "../World/WorldLoader.h"
#include "Input/InputHandler.h"
#include <future>
#include "glm/vec2.hpp"
#include "glm/gtx/compatibility.hpp"

namespace Game
{
    void GameScene::start()
    {
        auto* client = Engine::Client::getInstance();

        Scene::start();

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

        auto inventoryBg = spawn<Engine::ImageObject>("/Users/erhanguven/CLionProjects/growtopia_clone/growtopia_clone/Resources/ui/filled_square.png",10,true);
        inventoryBg->getTransform()->setScaleX(1.265f);
        inventoryBg->getTransform()->setScaleY(.3f);
        inventoryBg->getTransform()->setPositionY(-.75f);

        glm::vec4 c = {0,0.3f,0.5f,1};
        inventoryBg->getRenderer()->setColor(c);

        auto func = [&](const SyncVarTypeVariant& val, int connId)
        {
            spawn<Player>(connId, true);
            Engine::Client::getInstance()->callCommand("CMD_RequestWorld",0);
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

        auto onBlockSet = [&](const SyncVarTypeVariant& val, int connId)
        {
            //if(connId == Engine::Client::getInstance()->getConnectionId())
            //    return;

            auto msg = (glm::ivec3)std::get<glm::vec3>(val);
            blocks[msg.y][msg.z]->setBlockType((BlockType)msg.x, true);
        };
        client->getCommandController()->commands["RPC_OnBlockSet"].emplace_back(onBlockSet);

        auto fetchWorldFunc = [&](const SyncVarTypeVariant& val, int connId)
        {
            auto v = get<std::string>(val);
            m_LoadedWorldXml += v;
            printf("%s",v.c_str());
        };
        client->getCommandController()->commands["RPC_FetchWorld"].emplace_back(fetchWorldFunc);

        auto fetchWorldLastFunc = [&](const SyncVarTypeVariant& val, int connId)
        {
            auto v = get<std::string>(val);
            m_LoadedWorldXml += v;
            onFinishDownloadingWorld();
        };
        client->getCommandController()->commands["RPC_FetchWorldLast"].emplace_back(fetchWorldLastFunc);

        //World world = World();
        //WorldLoader::saveWorld("default", &world);

        client->connectTo("127.0.0.1",7777);
    }

    void GameScene::update(float dt)
    {
        Scene::update(dt);

        if(blocks.empty())
            return;

        float zoom = Engine::Camera::getDefault()->getZoom();

        int mouseX =(int)((Engine::InputHandler::mousePosition.x - 400.0f) / zoom + Engine::Camera::getDefault()->getPosition().x);
        int mouseY = (int)((Engine::InputHandler::mousePosition.y - 300.0f) / zoom - Engine::Camera::getDefault()->getPosition().y);
        int block_i = (mouseX+400)/50;
        int block_j = (int)(float(-mouseY+250)/50.0f + 1);

        bool blockExists = block_i >= 0 && block_i <= 15 && block_j >= 0 && block_j <= 7;

        if(blockExists && blocks[block_i][block_j] != nullptr)
        {
            if(Engine::InputHandler::isPressingMouseButton(GLFW_MOUSE_BUTTON_LEFT) && blocks[block_i][block_j]->getBlockType() != BlockType::Empty)
            {
                glm::vec2& playerPos =Player::myPlayer->getCharacterPosition();
                glm::vec2& blockPos =blocks[block_i][block_j]->getBlockPosition();

                printf("%d,%d. Distance to player = %f\n",block_i,block_j,glm::distance(playerPos,blockPos));

                if(glm::distance(playerPos,blockPos) < 120.0f)
                {
                    blocks[block_i][block_j]->takeDamage(dt * 100);
                }
            }
            else if(Engine::InputHandler::onPressMouseButton(GLFW_MOUSE_BUTTON_RIGHT))
            {
                printf("\nSet block\n");

                BlockType chosenBlockType = Player::myPlayer->getChosenInventoryItemData();
                glm::vec3 msg = {(int)chosenBlockType,block_i, block_j};

                if (chosenBlockType != BlockType::Empty)
                {
                    Player::myPlayer->useCurrentItem();
                    Engine::Client::getInstance()->callCommand("CMD_SetBlock", msg);
                }
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
            camOffset+=glm::vec2(150.0f*dt,0.0f);
        }
        if(Engine::InputHandler::isPressingKey(GLFW_KEY_LEFT))
        {
            camOffset-=glm::vec2(150.0f*dt,0.0f);
        }
        if(Engine::InputHandler::isPressingKey(GLFW_KEY_UP))
        {
            camOffset+=glm::vec2(0.0f,150.0f*dt);
        }
        if(Engine::InputHandler::isPressingKey(GLFW_KEY_DOWN))
        {
            camOffset-=glm::vec2(0.0f,150.0f*dt);
        }
        if(Engine::InputHandler::onPressKey(GLFW_KEY_C))
        {
            camOffset = glm::vec2(0.0f);
        }

    }

    void GameScene::lateUpdate(float dt)
    {
        Scene::lateUpdate(dt);
        if(Player::myPlayer)
        {
            float posX = Engine::Camera::getDefault()->getPosition().x;
            float posY = Engine::Camera::getDefault()->getPosition().y;

            if(Player::myPlayer->getCharacterPosition().y > -200)
                posY = Player::myPlayer->getCharacterPosition().y;
            if(Player::myPlayer->getCharacterPosition().x > -400 && Player::myPlayer->getCharacterPosition().x < 400)
                posX = Player::myPlayer->getCharacterPosition().x;

            glm::vec2 camPos = Engine::Camera::getDefault()->getPosition();
            glm::vec2 newPos = {posX, posY};
            newPos += camOffset;

            glm::vec2 result = glm::lerp(camPos, newPos, dt * 15);

            Engine::Camera::getDefault()->setPosition(result);
        }
    }

    void GameScene::onFinishDownloadingWorld()
    {
        auto* worldFromXml = new World(m_LoadedWorldXml);
        blocks = worldFromXml->getGrid();
    }
} // Game