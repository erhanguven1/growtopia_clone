//
// Created by erhan güven on 28.09.2023.
//

#include "Client.h"
#include <iostream>

namespace Engine
{
    int Client::connectTo(const char *ip_address, const int &port)
    {
        if (enet_initialize() != 0) {
            fprintf(stderr, "An error occurred while initializing ENet!\n");
            return EXIT_FAILURE;
        }
        atexit(enet_deinitialize);

        client = enet_host_create(nullptr, 1, 1, 0, 0);

        if (client == nullptr) {
            fprintf(stderr, "An error occurred while trying to create an ENet client host!\n");
            return EXIT_FAILURE;
        }

        ENetAddress address;
        ENetEvent event;

        enet_address_set_host(&address, ip_address);
        address.port = port;

        server = enet_host_connect(client, &address, 1, 0);
        if (server == nullptr) {
            fprintf(stderr, "No available peers for initiating an ENet connection!\n");
            return EXIT_FAILURE;
        }

        if (enet_host_service(client, &event, 5000) > 0 &&
            event.type == ENET_EVENT_TYPE_CONNECT)
        {
            puts("Connection to 127.0.0.1:7777 succeeded.");
            connectionId = event.peer->connectID;
            printf("Connection ID = %u",client->peers[0].connectID);

            if(!commandController.commands["RPC_OnConnect"].empty())
            {
                for(auto& cmd : commandController.commands["RPC_OnConnect"])
                {
                    cmd((int)connectionId,connectionId);
                }
            }
        } else {
            enet_peer_reset(server);
            puts("Connection to 127.0.0.1:7777 failed.");
            return EXIT_SUCCESS;
        }
    }

    void Client::receiveLoop()
    {
        ENetEvent event;

        while (enet_host_service(client, &event, 5000) > 0)
        {
            switch (event.type)
            {
                case ENET_EVENT_TYPE_RECEIVE:
                    onReceivePacket(event);
                    enet_packet_destroy(event.packet);
                    break;
            }
        }
    }

    void Client::onReceivePacket(const ENetEvent &event)
    {
        MsgData msgData;
        memcpy(&msgData, event.packet->data, event.packet->dataLength);

        auto msgType = (MessageTypes)msgData.type;

        switch (msgType)
        {
            case MessageTypes::PlayerData:
            {
                PlayerData playerData;
                memcpy(&playerData,msgData.data,sizeof(PlayerData));
                break;
            }
            case MessageTypes::SyncVar:
            {
                SyncVarData syncVarData;
                memcpy(&syncVarData,msgData.data,sizeof(SyncVarData));

                auto dataType = (SyncVarTypes)syncVarData.m_dataType;

                switch (dataType)
                {

                    case SyncVarTypes::INT:
                    {
                        int newVal;
                        memcpy(&newVal, syncVarData.data, sizeof(int));
                        syncVarHandler.fireEventWithId(syncVarData.m_id, newVal);
                        break;
                    }
                    case SyncVarTypes::FLOAT:
                    {
                        float newVal;
                        memcpy(&newVal, syncVarData.data, sizeof(float));
                        syncVarHandler.fireEventWithId(syncVarData.m_id, newVal);
                        break;
                    }
                    case SyncVarTypes::VEC2:
                        break;
                }

                break;
            }

            case MessageTypes::Command:
                break;
            case MessageTypes::ClientRPC:
                RemoteFunctionCallData rmCallData;
                memcpy(&rmCallData, msgData.data, sizeof(RemoteFunctionCallData));

                auto varType = (SyncVarTypes)rmCallData.m_parameterType;

                switch (varType)
                {
                    case SyncVarTypes::INT:
                    {
                        int param;
                        memcpy(&param, rmCallData.m_parameter, sizeof(int));
                        printf("%d", param);
                        break;
                    }
                    case SyncVarTypes::FLOAT:
                    {
                        float param;
                        memcpy(&param, rmCallData.m_parameter, sizeof(float));
                        printf("%f", param);
                        break;
                    }
                    case SyncVarTypes::VEC2:
                    {
                        glm::vec2 param;
                        memcpy(&param, rmCallData.m_parameter, sizeof(glm::vec2));
                        printf("%f,%f", param.x, param.y);
                        if(!commandController.commands[rmCallData.m_methodName].empty())
                        {
                            for(auto& cmd : commandController.commands[rmCallData.m_methodName])
                            {
                                cmd(param,rmCallData.receiverId);
                            }
                        }

                        break;
                    }
                }

                printf("\nreceived rpc!");
                break;
        }
    }

    void Client::callCommand(const char *methodName, const SyncVarTypeVariant& param)
    {
        RemoteFunctionCallData rmCallData;
        memcpy(rmCallData.m_methodName, methodName, strlen(methodName));
        rmCallData.m_parameterType = param.index();
        memcpy(rmCallData.m_parameter, &param, sizeof(param));

        MsgData msgData;
        msgData.type = (int)MessageTypes::Command;
        memcpy(msgData.data, &rmCallData, sizeof(RemoteFunctionCallData));

        auto packet = enet_packet_create(&msgData,sizeof(uint)+sizeof(RemoteFunctionCallData),NULL);

        enet_peer_send(server,0,packet);
    }
} // Engine