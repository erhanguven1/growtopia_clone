//
// Created by erhan güven on 29.10.2023.
//

#ifndef GROWTOPIA_CLONE_PLAYER_H
#define GROWTOPIA_CLONE_PLAYER_H

#include "Gameobject/ImageObject.h"

namespace Game
{
    class Player : public Engine::Gameobject
    {
    public:
        Player(uint connId, bool isMine);
        inline static Player* myPlayer = nullptr;
        void start() override{}
        void update(float) override;
        inline glm::vec2& getCharacterPosition()
        {
            return character->getTransform()->getPosition();
        }
        inline Engine::Transform& getCharacterTransform()
        {
            return *character->getTransform();
        }
    private:
        Engine::ImageObject* character;
        uint connectionId;
        void RPC_UpdatePlayerPosition(const std::variant<int, float, glm::vec2> &val, int connId) const;
        void RPC_UpdatePlayerFacingDirection(const std::variant<int, float, glm::vec2> &val, int connId) const;
        bool m_isMine;
    };

} // Game

#endif //GROWTOPIA_CLONE_PLAYER_H
