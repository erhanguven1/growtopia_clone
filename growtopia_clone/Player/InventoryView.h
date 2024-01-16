//
// Created by erhan güven on 16.01.2024.
//

#ifndef GROWTOPIA_CLONE_INVENTORYVIEW_H
#define GROWTOPIA_CLONE_INVENTORYVIEW_H

#include "InventoryData.h"
#include "Gameobject/Button.h"

namespace Game
{
    class InventoryItemButton;
    class InventoryController;
    class InventoryView
    {
        friend class InventoryItemButton;
    public:
        InventoryView()=delete;
        InventoryView(std::vector<InventoryItemData*> inventoryItems, InventoryController* inventoryController);
        void deleteItem(BlockType blockType);
    private:
        std::unordered_map<BlockType,InventoryItemButton*> inventoryItemButtons;
    };

    class InventoryItemButton : public Engine::Button
    {
    public:
        InventoryItemButton(const char* path, InventoryItemData* invItemData, InventoryController& inventoryController);

    private:
        InventoryItemData* m_inventoryItemData;
    };

} // Game

#endif //GROWTOPIA_CLONE_INVENTORYVIEW_H
