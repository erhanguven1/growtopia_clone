//
// Created by erhan güven on 16.01.2024.
//

#ifndef GROWTOPIA_CLONE_INVENTORYVIEW_H
#define GROWTOPIA_CLONE_INVENTORYVIEW_H

#include "InventoryData.h"

namespace Game
{

    class InventoryView
    {
    public:
        InventoryView()=delete;
        InventoryView(const std::vector<InventoryItemData*>& inventoryItems);
    private:

    };

} // Game

#endif //GROWTOPIA_CLONE_INVENTORYVIEW_H
