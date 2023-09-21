//
// Created by erhan güven on 16.09.2023.
//

#include <EngineStarter.hpp>
#include "Scenes/MainMenuScene.h"

int main()
{
    EngineStarter engineStarter;
    engineStarter.initializeGame(Engine::Resolutions::_800x600, "Growtopia Clone");

    using namespace Game;

    MainMenuScene mainMenuScene = MainMenuScene();
    Engine::SceneManager::setCurrentScene(mainMenuScene);

    engineStarter.loop();
    engineStarter.exit();

    return 0;
}