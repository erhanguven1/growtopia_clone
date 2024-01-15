//
// Created by erhan güven on 15.01.2024.
//

#include "Inventory.h"
#include <tinyxml2/tinyxml2.h>
#include <string>

namespace Game
{
    Inventory::Inventory(const std::string& xmlInventoryData)
    {
        using namespace tinyxml2;

        auto* doc = new XMLDocument();
        doc->Parse(xmlInventoryData.c_str());

        auto* inventoryDataElement = doc->FirstChildElement("InventoryData");

        auto* blockElement = inventoryDataElement->FirstChildElement();

        while (blockElement != nullptr)
        {
            const char *tileType = blockElement->Attribute("Type");
            auto blockType = (BlockType)atoi(tileType);
            int count = atoi(blockElement->FirstChildElement("Count")->GetText());

            auto* invItem = new InventoryItem(blockType, count);

            m_inventoryItems.push_back(invItem);

            blockElement = blockElement->NextSiblingElement();
        }
    }
} // Game