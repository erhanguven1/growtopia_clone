//
// Created by erhan güven on 16.01.2024.
//

#include "InventoryController.h"
#include "Networking/Client.h"

namespace Game
{
    InventoryController::InventoryController(int connId)
    {
        auto* client = Engine::Client::getInstance();
        client->callCommand("CMD_RequestInventory", connId);

        auto onFetchInventory = [&](const SyncVarTypeVariant& val, int connId)
        {
            auto v = get<std::string>(val);
            m_LoadedInventoryXml += v;
        };
        client->getCommandController()->commands["RPC_FetchInventory"].emplace_back(onFetchInventory);

        auto onFetchInventoryLast = [&](const SyncVarTypeVariant& val, int connId)
        {
            auto v = get<std::string>(val);
            m_LoadedInventoryXml += v;

            onFinishedDownloadingXml();
        };
        client->getCommandController()->commands["RPC_FetchInventoryLast"].emplace_back(onFetchInventoryLast);
    }

    void InventoryController::onFinishedDownloadingXml()
    {
        inventoryData = std::make_unique<InventoryData>(m_LoadedInventoryXml);
        inventoryView = std::make_unique<InventoryView>(inventoryData->m_inventoryItems);
    }
} // Game