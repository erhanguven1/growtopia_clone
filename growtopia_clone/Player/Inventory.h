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
        InventoryItem(BlockType blockType, unsigned int count) : m_blockType(blockType), m_count(count) {}
    private:
        BlockType m_blockType = BlockType::Empty;
        unsigned int m_count = 0;
    };

    class Inventory
    {
    public:
        explicit Inventory(const std::string& xmlInventoryData);
    private:
        std::vector<InventoryItem*> m_inventoryItems;
    };

} // Game

#endif //GROWTOPIA_CLONE_INVENTORY_H
