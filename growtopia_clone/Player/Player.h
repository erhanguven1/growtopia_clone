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
        explicit Player(uint connId);
        inline static Player* myPlayer = nullptr;
        void start() override{}
        void update(float) override;
    private:
        Engine::ImageObject* character = nullptr;
        uint connectionId;
        void RPC_UpdatePlayerPosition(const std::variant<int, float, glm::vec2> &val, int connId) const;
    };

} // Game

#endif //GROWTOPIA_CLONE_PLAYER_H
