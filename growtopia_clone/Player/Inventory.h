//
// Created by erhan güven on 15.01.2024.
//

#ifndef GROWTOPIA_CLONE_INVENTORY_H
#define GROWTOPIA_CLONE_INVENTORY_H

#include <Common/BlockTypes.h>
#include <vector>

namespace Game
{

    class InventoryItem
    {
    public:
        InventoryItem() = delete;
    private:
        BlockType m_blockType = BlockType::Empty;
        unsigned int count = 0;
    };

    class Inventory
    {
    public:
        Inventory(const std::string& xmlInventoryData);
    private:
        std::vector<InventoryItem*> inventoryItems;
    };

} // Game

#endif //GROWTOPIA_CLONE_INVENTORY_H
