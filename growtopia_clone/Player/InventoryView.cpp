//
// Created by erhan güven on 16.01.2024.
//

#include "InventoryView.h"
#include "Gameobject/Button.h"
#include "Scene/SceneManager.h"

namespace Game
{
    InventoryView::InventoryView(const std::vector<InventoryItemData *>& inventoryItems)
    {
        for (int i = 0; i < 5; ++i)
        {
            auto frame = Engine::SceneManager::getCurrentScene()->spawn<Engine::ImageObject>("/Users/erhanguven/CLionProjects/growtopia_clone/growtopia_clone/Resources/ui/square_ui.png",10,true);
            frame->getTransform()->setPositionX(-.495f + i * .25f);
            frame->getTransform()->setPositionY(-.75f);
            frame->getTransform()->setScaleX(.25f*.75f);
            frame->getTransform()->setScaleY(.25f);
        }

        for (int i = 0; i < inventoryItems.size(); ++i)
        {
            std::string path = "/Users/erhanguven/CLionProjects/growtopia_clone/growtopia_clone/Resources/";
            path += BlockTypeHelper::getBlockFilename(inventoryItems[i]->getBlockType());

            auto icon = Engine::SceneManager::getCurrentScene()->spawn<Engine::Button>(path.c_str(),10);
            icon->getTransform()->setPositionX(-.495f + i * .25f);
            icon->getTransform()->setPositionY(-.75f);
            icon->getTransform()->setScaleX(.2f*.75f);
            icon->getTransform()->setScaleY(.2f);
        }
    }
} // Game