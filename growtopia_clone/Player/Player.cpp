//
// Created by erhan güven on 29.10.2023.
//

#include "Player.h"
#include "Networking/Client.h"
#include "Scene/SceneManager.h"
#include <Input/InputHandler.h>

namespace Game
{
    Player::Player(uint connId) : Gameobject(99), connectionId(connId)
    {
        character = Engine::SceneManager::getCurrentScene()->spawn<Engine::ImageObject>(1);
        glm::vec2 scale = glm::vec2(.25f,.5f);
        character->getTransform()->setScale(scale);
        character->getTransform()->setIsRigidBody(true);
        character->getTransform()->setPositionY(1.0f);

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
            glm::vec2 position = (std::get<glm::vec2>(val));
            printf("\nposition= %f,%f",position.x,position.y);
            character->getTransform()->setPositionX(position.x);
        }
    }

    void Player::update(float dt)
    {
        auto client = Engine::Client::getInstance();
        if(Engine::InputHandler::isPressingKey(GLFW_KEY_D))
        {
            //client->callCommand("CMD_MoveTo", glm::vec2(.01f,0));
            auto v = glm::vec2(.01f,0);
            character->getTransform()->addToPosition(v);
        }
        if(Engine::InputHandler::isPressingKey(GLFW_KEY_A))
        {
            //client->callCommand("CMD_MoveTo", glm::vec2(-.01f,0));
            auto v = glm::vec2(-.01f,0);
            character->getTransform()->addToPosition(v);
        }
        if(Engine::InputHandler::onPressKey(GLFW_KEY_W))
        {
            glm::vec2 up = glm::vec2(0,1000.0f);
            character->getTransform()->getRigidBody().setVelocity(up);
        }
    }
} // Game