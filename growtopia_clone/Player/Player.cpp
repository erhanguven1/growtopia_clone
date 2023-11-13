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
        character = Engine::SceneManager::getCurrentScene()->spawn<Engine::ImageObject>(1);
        glm::vec2 scale = glm::vec2(.25f,.5f);
        character->getTransform()->setScale(scale);
        character->getTransform()->setIsRigidBody(m_isMine);
        character->getTransform()->setPositionY(1.0f);
        character->getTransform()->hasCollider = true;

        auto* client = Engine::Client::getInstance();
        if(client->getConnectionId() == connectionId)
        {
            Player::myPlayer = this;
        }
        auto func = [&](const SyncVarTypeVariant& val, int connId)
        {
            RPC_UpdatePlayerPosition(val, connId);
        };
        client->getCommandController()->commands["RPC_UpdatePlayerPosition"].emplace_back(func);
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

    void Player::update(float dt)
    {
        auto client = Engine::Client::getInstance();
        if(client->getConnectionId() == connectionId)
        {
            if(Engine::InputHandler::isPressingKey(GLFW_KEY_D))
            {
                auto v = glm::vec2(.01f,0);
                character->getTransform()->addToPosition(v);
            }
            if(Engine::InputHandler::isPressingKey(GLFW_KEY_A))
            {
                auto v = glm::vec2(-.01f,0);
                character->getTransform()->addToPosition(v);
            }
            if(Engine::InputHandler::onPressKey(GLFW_KEY_W))
            {
                auto v = glm::vec2(0,1000.0f);
                character->getTransform()->getRigidBody().setVelocity(v);
            }

            client->callCommand("CMD_MoveTo", character->getTransform()->getPosition());
        }
    }
} // Game