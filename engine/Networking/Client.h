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
        std::unordered_map<std::string, std::vector<std::function<void(const SyncVarTypeVariant&, uint)>>> commands;
    };

    class Client final
    {
    public:
        inline SyncVarHandler* getSyncVarHandler()
        {
            return &syncVarHandler;
        }

        inline CommandController* getCommandController()
        {
            return &commandController;
        }

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
        void receiveLoop();
        void callCommand(const char* methodName, const SyncVarTypeVariant& param);

        inline uint getConnectionId() const{return connectionId;}

        void callRPCs();
        void callRPC(RemoteFunctionCallData& rmCallData);
        std::vector<RemoteFunctionCallData*> queuedCalls;

    private:
        Client(){}
        inline static Client* instance = nullptr;
        SyncVarHandler syncVarHandler;
        CommandController commandController;
        ENetPeer* server;
        ENetHost *client;

        uint connectionId;

        void onReceivePacket(const ENetEvent& event);
    };

} // Engine

#endif //GROWTOPIA_CLONE_CLIENT_H
