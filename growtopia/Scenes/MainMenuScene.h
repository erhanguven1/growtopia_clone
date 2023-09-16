//
// Created by erhan güven on 16.09.2023.
//

#ifndef GROWTOPIA_CLONE_MAINMENUSCENE_H
#define GROWTOPIA_CLONE_MAINMENUSCENE_H

#include <Scene/SceneManager.h>

namespace Game
{

    class MainMenuScene : public Engine::Scene
    {
    public:
        void start() override;
        void update(float) override;
    };

} // Game

#endif //GROWTOPIA_CLONE_MAINMENUSCENE_H
