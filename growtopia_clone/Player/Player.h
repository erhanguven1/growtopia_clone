//
// Created by erhan güven on 29.10.2023.
//

#ifndef GROWTOPIA_CLONE_PLAYER_H
#define GROWTOPIA_CLONE_PLAYER_H

#include "Gameobject/ImageObject.h"
#include "Inventory.h"

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
    private:
        std::unique_ptr<Engine::ImageObject> character = nullptr;
        std::unique_ptr<Inventory> inventory = nullptr;
        uint connectionId;
        bool m_isMine;
        void RPC_UpdatePlayerPosition(const std::variant<int, float, glm::vec2, std::string> &val, int connId) const;
        void RPC_UpdatePlayerFacingDirection(const std::variant<int, float, glm::vec2, std::string> &val, int connId) const;
        std::string m_LoadedInventoryXml = "";
    };

} // Game

#endif //GROWTOPIA_CLONE_PLAYER_H
