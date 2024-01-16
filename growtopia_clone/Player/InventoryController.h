//
// Created by erhan güven on 16.01.2024.
//

#ifndef GROWTOPIA_CLONE_INVENTORYCONTROLLER_H
#define GROWTOPIA_CLONE_INVENTORYCONTROLLER_H

#include "InventoryData.h"
#include "InventoryView.h"
#include <string>

namespace Game
{

    class InventoryController
    {
    public:
        InventoryController()=delete;
        explicit InventoryController(int connId);
    private:
        std::unique_ptr<InventoryData> inventoryData;
        std::unique_ptr<InventoryView> inventoryView;

        std::string m_LoadedInventoryXml = "";

        void onFinishedDownloadingXml();
    };

} // Game

#endif //GROWTOPIA_CLONE_INVENTORYCONTROLLER_H
