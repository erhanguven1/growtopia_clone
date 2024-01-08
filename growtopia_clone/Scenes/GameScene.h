//
// Created by erhan güven on 1.12.2023.
//

#ifndef GROWTOPIA_CLONE_GAMESCENE_H
#define GROWTOPIA_CLONE_GAMESCENE_H

#include "Scene/Scene.h"
#include "../Player/Player.h"
#include "../World/Tile.h"

namespace Game
{

    class GameScene : public Engine::Scene
    {
    public:
        GameScene() : Engine::Scene(1)
        {

        }
        void start() override;
        void update(float) override;
        void lateUpdate(float) override;
        inline static std::string worldName;
    private:
        std::vector<Player*> players;
        std::vector<std::vector<Tile*>> blocks;

        std::string m_LoadedWorldXml = "";
        void onFinishDownloadingWorld();
    };

} // Game

#endif //GROWTOPIA_CLONE_GAMESCENE_H
