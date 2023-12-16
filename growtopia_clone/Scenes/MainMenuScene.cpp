//
// Created by erhan güven on 16.09.2023.
//

#include "MainMenuScene.h"
#include "Gameobject/Button.h"
#include "GameScene.h"
#include <Input/InputHandler.h>

namespace Game
{
    void MainMenuScene::start()
    {
        Scene::start();

        auto bg = spawn<Engine::ImageObject>("/Users/erhanguven/CLionProjects/growtopia_clone/growtopia_clone/Resources/backgrounds/bg-0.png",0);
        bg->getTransform()->setScaleX(800.0f*1.5f);
        bg->getTransform()->setScaleY(419.735259f*1.5f);
        bg->getTransform()->setPositionY(1.0f/3.0f);

        auto playOnlineButton = spawn<Engine::Button>("/Users/erhanguven/CLionProjects/growtopia_clone/growtopia_clone/Resources/ui/play_online_button.png");
        playOnlineButton->getTransform()->setPositionY(.25f);
        playOnlineButton->getTransform()->setScaleX(.15f*4.37931034483f);
        playOnlineButton->getTransform()->setScaleY(.2f);

        std::function<void()> onClickPlayOnline = [](){
            Engine::SceneManager::setCurrentScene(1);
        };

        playOnlineButton->registerOnClick(onClickPlayOnline);
    }

    void MainMenuScene::update(float dt)
    {
        Scene::update(dt);
        if(Engine::InputHandler::onPressKey(GLFW_KEY_1))
        {
            GameScene::worldName = "world0";
        }
        if(Engine::InputHandler::onPressKey(GLFW_KEY_2))
        {
            GameScene::worldName = "world1";
        }
        if(Engine::InputHandler::onPressKey(GLFW_KEY_3))
        {
            GameScene::worldName = "world2";
        }
    }
} // Game