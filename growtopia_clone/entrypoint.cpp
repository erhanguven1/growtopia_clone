//
// Created by erhan güven on 16.09.2023.
//

#include <EngineStarter.hpp>
#include "Scenes/MainMenuScene.h"
#include <future>

int main()
{
    EngineStarter engineStarter;
    engineStarter.initializeGame(Engine::Resolutions::_800x600, "Growtopia Clone");

    auto* client = Engine::Client::getInstance();
    client->connectTo("127.0.0.1",7777);
    auto t = std::async(std::launch::async, &Engine::Client::recvLoop, client);

    using namespace Game;

    MainMenuScene mainMenuScene = MainMenuScene();
    Engine::SceneManager::setCurrentScene(mainMenuScene);

    engineStarter.loop();
    engineStarter.exit();

    return 0;
}