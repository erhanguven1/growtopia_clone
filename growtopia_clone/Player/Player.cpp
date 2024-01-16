//
// Created by erhan güven on 29.10.2023.
//

#include "Player.h"
#include "Networking/Client.h"
#include "Scene/SceneManager.h"
#include <Input/InputHandler.h>

namespace Game
{
    Player::Player(uint connId, bool isMine) : Gameobject(99), connectionId(connId), m_isMine(isMine)
    {
        character = std::unique_ptr<Engine::ImageObject>(Engine::SceneManager::getCurrentScene()->spawn<Engine::ImageObject>("/Users/erhanguven/CLionProjects/growtopia_clone/growtopia_clone/Resources/base_skin.png",99));
        glm::vec2 scale = glm::vec2(34.0f,50.0f);
        character->getTransform()->setScale(scale);
        character->getTransform()->setIsRigidBody(m_isMine);
        character->getTransform()->setPositionY(1.0f);
        character->getTransform()->hasCollider = true;

        auto* client = Engine::Client::getInstance();
        if(client->getConnectionId() == connectionId)
        {
            Player::myPlayer = this;
        }
        auto onUpdatePlayerPosition = [&](const SyncVarTypeVariant& val, int connId)
        {
            RPC_UpdatePlayerPosition(val, connId);
        };
        client->getCommandController()->commands["RPC_UpdatePlayerPosition"].emplace_back(onUpdatePlayerPosition);
        auto onUpdatePlayerFacingDirection = [&](const SyncVarTypeVariant& val, int connId)
        {
            RPC_UpdatePlayerFacingDirection(val, connId);
        };
        client->getCommandController()->commands["RPC_UpdatePlayerFacingDirection"].emplace_back(onUpdatePlayerFacingDirection);


        if (m_isMine)
        {
            inventoryController = std::make_unique<InventoryController>(connId);
        }
    }

    void Player::RPC_UpdatePlayerPosition(const SyncVarTypeVariant& val, int connId) const
    {
        if(connId == connectionId)
        {
            glm::vec2 newPosition = (std::get<glm::vec2>(val));
            character->getTransform()->setPositionX(newPosition.x);
            character->getTransform()->setPositionY(newPosition.y);
        }
    }

    void Player::RPC_UpdatePlayerFacingDirection(const std::variant<int, float, glm::vec2, std::string> &val, int connId) const
    {
        if(connId == connectionId)
        {
            character->getTransform()->mirror = std::get<int>(val);
        }
    }

    void Player::update(float dt)
    {
        auto client = Engine::Client::getInstance();
        if(client->getConnectionId() == connectionId)
        {
            if(Engine::InputHandler::isPressingKey(GLFW_KEY_D))
            {
                auto v = glm::vec2(2.5f,0);
                character->getTransform()->addToPosition(v);
                if(!character->getTransform()->mirror)
                {
                    client->callCommand("CMD_LookAt", 1);
                }
                character->getTransform()->mirror = true;
            }
            if(Engine::InputHandler::isPressingKey(GLFW_KEY_A))
            {
                auto v = glm::vec2(-2.5f,0);
                character->getTransform()->addToPosition(v);
                if(character->getTransform()->mirror)
                {
                    client->callCommand("CMD_LookAt", 0);
                }
                character->getTransform()->mirror = false;
            }
            if(Engine::InputHandler::onPressKey(GLFW_KEY_W))
            {
                auto v = glm::vec2(0,500.0f);
                character->getTransform()->jumpExtraMultiplier = 1.0f;
                character->getTransform()->getRigidBody()->setVelocity(v);
            }

            client->callCommand("CMD_MoveTo", character->getTransform()->getPosition());

        }
    }
} // Game