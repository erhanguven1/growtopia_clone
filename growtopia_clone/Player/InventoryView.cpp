//
// Created by erhan güven on 16.01.2024.
//

#include "InventoryView.h"
#include "Gameobject/Button.h"
#include "Scene/SceneManager.h"
#include "InventoryController.h"

namespace Game
{
    InventoryView::InventoryView(std::vector<InventoryItemData *> inventoryItems, InventoryController* inventoryController)
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

            auto icon = Engine::SceneManager::getCurrentScene()->spawn<InventoryItemButton>(path.c_str(), inventoryItems[i],
                                                                                            *inventoryController);
            icon->getTransform()->setPositionX(-.495f + i * .25f);
            icon->getTransform()->setPositionY(-.75f);
            icon->getTransform()->setScaleX(.2f*.75f);
            icon->getTransform()->setScaleY(.2f);

            inventoryItemButtons.push_back(icon);
        }
    }

    InventoryItemButton::InventoryItemButton(const char *path, InventoryItemData *invItemData,
                                             InventoryController &inventoryController) : m_inventoryItemData(invItemData), Engine::Button(path, 10)
    {
        std::function<void()> onClick = [&]()
        {
            inventoryController.onClickInventoryItem(*m_inventoryItemData);
        };
        registerOnClick(onClick);
    }
} // Game