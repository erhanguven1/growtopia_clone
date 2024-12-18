//
// Created by erhan güven on 16.09.2023.
//

#ifndef GROWTOPIA_CLONE_MAINMENUSCENE_H
#define GROWTOPIA_CLONE_MAINMENUSCENE_H

#include <Scene/SceneManager.h>
#include "Gameobject/ImageObject.h"
#include "Networking/Client.h"
#include "../Player/Player.h"
#include "../World/Tile.h"

namespace Game
{

    class MainMenuScene : public Engine::Scene
    {
    public:
        MainMenuScene() : Engine::Scene(0)
        {

        }
        void start() override;
        void update(float) override;
    };

} // Game

#endif //GROWTOPIA_CLONE_MAINMENUSCENE_H
