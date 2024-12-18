//
// Created by erhan güven on 15.01.2024.
//

#ifndef GROWTOPIA_CLONE_INVENTORYDATA_H
#define GROWTOPIA_CLONE_INVENTORYDATA_H

#include <Common/BlockTypes.h>
#include <vector>

namespace Game
{

    class InventoryItemData
    {
    public:
        InventoryItemData() = delete;
        InventoryItemData(BlockType blockType, unsigned int count) : m_blockType(blockType), m_count(count) {}
        InventoryItemData(const InventoryItemData& inventoryItemData)
        {

        }

        inline BlockType getBlockType() { return m_blockType; }
        inline unsigned int getCount() const { return m_count; }

        inline void decreaseItemCount(unsigned int count)
        {
            if (m_count > 0)
                m_count--;

            printf("\n%d 's new count: %d\n", m_blockType, m_count);
        }

    private:
        BlockType m_blockType = BlockType::Empty;
        unsigned int m_count = 0;
    };

    class InventoryData
    {
        friend class InventoryController;
    public:
        explicit InventoryData(const std::string& xmlInventoryData);
    private:
        std::vector<InventoryItemData*> m_inventoryItems;
    };

} // Game

#endif //GROWTOPIA_CLONE_INVENTORYDATA_H
