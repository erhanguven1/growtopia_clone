//
// Created by erhan güven on 16.09.2023.
//

#include <EngineStarter.hpp>
#include "Scenes/MainMenuScene.h"
#include "Scenes/GameScene.h"
#include <future>

int main()
{
    EngineStarter engineStarter;
    engineStarter.initializeGame(Engine::Resolutions::_800x600, "Growtopia Clone");

    using namespace Game;

    MainMenuScene mainMenuScene = MainMenuScene();
    Engine::SceneManager::addScene(mainMenuScene.getSceneId(), mainMenuScene);
    GameScene gameScene = GameScene();
    Engine::SceneManager::addScene(gameScene.getSceneId(), gameScene);
    Engine::SceneManager::setCurrentScene(mainMenuScene.getSceneId());

    auto* client = Engine::Client::getInstance();
    auto t = std::async(std::launch::async, &Engine::Client::receiveLoop, client);

    engineStarter.loop();
    engineStarter.exit();

    return 0;
}