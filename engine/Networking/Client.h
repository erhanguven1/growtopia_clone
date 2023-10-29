//
// Created by erhan güven on 28.09.2023.
//

#ifndef GROWTOPIA_CLONE_CLIENT_H
#define GROWTOPIA_CLONE_CLIENT_H

#include <enet/enet.h>
#include "NetworkStructs.h"
#include <functional>
#include <iostream>

namespace Engine
{
    using SyncVarFuncs = std::variant<std::function<void(int)>, std::function<void(float)>>;

    class SyncVarHandler
    {
    public:
        void registerSyncVar(const SyncVarFuncs& func)
        {
            events.push_back(func);
        }

        template<class T>
        void fireEventWithId(uint id, T& var)
        {
            std::get<std::function<void(T)>>(events[id])(var);
        }
    private:
        std::vector<SyncVarFuncs> events;
    };

    class CommandController
    {
    public:
        std::unordered_map<std::string, std::function<void(const SyncVarTypeVariant&, bool)>> commands;
    };

    class Client final
    {
    public:
        SyncVarHandler syncVarHandler;
        CommandController commandController;

        inline static Client* getInstance()
        {
            if(instance == nullptr)
            {
                printf("Instance is null, creating a new one!");
                instance = new Client;
            }
            return instance;
        }

        int connectTo(const char* ip_address, const int& port);
        void recvLoop();
        void callCommand(const char* methodName, const SyncVarTypeVariant& param);
        void sendPlayerData(const PlayerData& playerData);

    private:
        void RPC_UpdatePlayerPosition(const SyncVarTypeVariant& val, bool isMine);

    private:
        Client(){}
        inline static Client* instance = nullptr;
        ENetPeer* server;
        ENetHost *client;

        void onReceivePacket(const ENetEvent& event);
    };

} // Engine

#endif //GROWTOPIA_CLONE_CLIENT_H
