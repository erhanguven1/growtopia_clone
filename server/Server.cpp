#include <cstdio>
#include <enet/enet.h>
#include <cstring>
#include <Networking/NetworkStructs.h>
#include <unordered_map>
#include <vector>
#include <string>
#include <tinyxml2/tinyxml2.h>

void loadWorld(const char* worldName, std::string& worldData, size_t& worldDataLength);
void loadInventory(const char* playerName, std::string& inventoryData, size_t& inventoryDataLength);
void onReceivePacket(const ENetEvent& event);
void registerRpcMethodsToCommands();
void CMD_RequestWorld(const SyncVarTypeVariant& val, int);
void CMD_RequestInventory(const SyncVarTypeVariant&, int);
void CMD_MoveTo(const SyncVarTypeVariant&, int);
void CMD_LookAt(const SyncVarTypeVariant&, int);
void CMD_DestroyBlock(const SyncVarTypeVariant&, int);
void CMD_SetBlock(const SyncVarTypeVariant&, int);

std::unordered_map<enet_uint32 , ENetPeer*> connections;
std::vector<SyncVarTypeVariant> variables;

std::string WorldData;
std::string InventoryData;

class CommandController
{
public:
    std::unordered_map<std::string, void(*)(const SyncVarTypeVariant&, int)> commands;
};

static CommandController* cmdController;
ENetHost* server;

int main (int argc, char ** argv)
{
    cmdController = new CommandController;
    if (enet_initialize () != 0)
    {
        fprintf (stderr, "An error occurred while initializing ENet.\n");
        return EXIT_FAILURE;
    }
    atexit (enet_deinitialize);

    ENetAddress address;

    /* Bind the server to the default localhost.     */
    /* A specific host address can be specified by   */
    /* enet_address_set_host (& address, "x.x.x.x"); */
    address.host = ENET_HOST_ANY;
    /* Bind the server to port 7777. */
    address.port = 7777;

    server = enet_host_create (&address	/* the address to bind the server host to */,
                               32	/* allow up to 32 clients and/or outgoing connections */,
                               1	/* allow up to 1 channel to be used, 0. */,
                               0	/* assume any amount of incoming bandwidth */,
                               0	/* assume any amount of outgoing bandwidth */);

    if (server == NULL)
    {
        printf("An error occurred while trying to create an ENet server host.");
        return 1;
    }

    registerRpcMethodsToCommands();

    std::chrono::steady_clock::time_point lastUpdate;

    ENetEvent event;
    //enet_host_service (server, &event, 10000);
    // gameloop
    while(true)
    {
        while (enet_host_service(server, &event, 30) > 0)
        {
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT: {
                    printf("A new client connected from %x:%u.\n",
                           event.peer->address.host,
                           event.peer->address.port);

                    uint * cidPtr = new uint;
                    *cidPtr = event.peer->connectID;

                    event.peer->data = cidPtr;

                    uint cid = 1;
                    memcpy(&cid, event.peer->data, sizeof(uint));

                    for(auto conn : connections)
                    {
                        {
                            //Let the newcomer know others
                            MsgData msgData;
                            msgData.type = (uint) (MessageTypes::ClientRPC);

                            RemoteFunctionCallData rmData;
                            rmData.m_parameterType = (uint) SyncVarTypes::INT;
                            rmData.parameterSize = sizeof(int);
                            rmData.receiverId = event.peer->connectID;
                            memcpy(rmData.m_parameter, &conn.second->connectID, sizeof(conn.second->connectID));
                            memcpy(rmData.m_methodName, "RPC_OnFetchOtherPlayer", strlen("RPC_OnFetchOtherPlayer"));

                            memcpy(msgData.data, &rmData, sizeof(rmData));

                            auto packet = enet_packet_create(&msgData, sizeof(uint) + sizeof(RemoteFunctionCallData),
                                                             NULL);
                            enet_peer_send(event.peer, 0, packet);
                        }

                        {
                            //Let others know the newcomer
                            MsgData msgData2;
                            msgData2.type = (uint)(MessageTypes::ClientRPC);

                            RemoteFunctionCallData rmData2;
                            rmData2.m_parameterType = (uint)SyncVarTypes::INT;
                            rmData2.receiverId = conn.second->connectID;
                            rmData2.parameterSize = sizeof(int);
                            memcpy(rmData2.m_parameter, &event.peer->connectID, sizeof(event.peer->connectID));
                            memcpy(rmData2.m_methodName, "RPC_FetchNewcomer", strlen("RPC_FetchNewcomer"));

                            memcpy(msgData2.data, &rmData2, sizeof(rmData2));

                            auto packet2 = enet_packet_create(&msgData2,sizeof(uint)+sizeof(RemoteFunctionCallData),NULL);
                            enet_peer_send(conn.second, 0, packet2);
                        }
                    }

                    connections[event.peer->connectID] = event.peer;
                    printf("Connection ID: %u\n", event.peer->connectID);

                    break;
                }
                case ENET_EVENT_TYPE_RECEIVE:
                {
                    uint cid = 0;
                    memcpy(&cid, event.peer->data, sizeof(uint));
                    printf("A packet of length %u containing %s was received from %s on channel %u, rtt %u.\n",
                           event.packet->dataLength,
                           event.packet->data,
                           event.peer->data,
                           event.channelID,
                           event.peer->roundTripTime);

                    onReceivePacket(event);
                    /* Clean up the packet now that we're done using it. */
                    enet_packet_destroy(event.packet);
                    break;
                }
                case ENET_EVENT_TYPE_DISCONNECT:
                {
                    uint cid = 0;
                    memcpy(&cid, event.peer->data, sizeof(uint));

                    printf("%u disconnected.\n", cid);

                    connections.erase(cid);
                    /* Reset the peer's client information. */
                    event.peer->data = nullptr;
                }
                case ENET_EVENT_TYPE_NONE:
                    break;
            }
        }
    }

    enet_host_destroy(server);

    return 0;
}

void onReceivePacket(const ENetEvent& event)
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

            break;
        }
        case MessageTypes::Command:
        {

            RemoteFunctionCallData rmCallData;
            memcpy(&rmCallData, msgData.data, sizeof(RemoteFunctionCallData));

            auto varType = (SyncVarTypes)rmCallData.m_parameterType;

            switch (varType)
            {
                case SyncVarTypes::INT:
                {
                    int param;
                    memcpy(&param, rmCallData.m_parameter, sizeof(int));
                    cmdController->commands.at(rmCallData.m_methodName)(param, event.peer->connectID);
                    break;
                }
                case SyncVarTypes::FLOAT:
                {
                    float param;
                    memcpy(&param, rmCallData.m_parameter, sizeof(float));
                    cmdController->commands.at(rmCallData.m_methodName)(param, event.peer->connectID);
                    break;
                }
                case SyncVarTypes::VEC2:
                {
                    glm::vec2 param;
                    memcpy(&param, rmCallData.m_parameter, sizeof(glm::vec2));
                    cmdController->commands.at(rmCallData.m_methodName)(param, event.peer->connectID);
                    break;
                }
                case SyncVarTypes::VEC3:
                {
                    glm::vec3 param;
                    memcpy(&param, rmCallData.m_parameter, sizeof(glm::vec3));
                    cmdController->commands.at(rmCallData.m_methodName)(param, event.peer->connectID);
                    break;
                }
            }

            break;
        }
        case MessageTypes::ClientRPC:
            fprintf(stderr, "Server cannot get ClientRPC messages!\n");
            break;
    }
}

void loadWorld(const char* worldName, std::string& worldData, size_t& worldDataLength)
{
    const std::string fullPath = std::string("/Users/erhanguven/CLionProjects/growtopia_clone/growtopia_clone/Resources/Worlds/")+worldName+".xml";

    using namespace tinyxml2;

    auto* doc = new XMLDocument();
    doc->LoadFile(fullPath.c_str());

    XMLPrinter printer;
    doc->Accept(&printer);

    worldData = printer.CStr();

    worldDataLength = worldData.length();
}

void loadInventory(const char* playerName, std::string& inventoryData, size_t& inventoryDataLength)
{
    const std::string fullPath = std::string("/Users/erhanguven/CLionProjects/growtopia_clone/growtopia_clone/Resources/Inventories/")+playerName+".xml";

    using namespace tinyxml2;

    auto* doc = new XMLDocument();
    doc->LoadFile(fullPath.c_str());


    XMLPrinter printer;
    doc->Accept(&printer);

    inventoryData = printer.CStr();

    inventoryDataLength = inventoryData.length();
}

void registerRpcMethodsToCommands()
{
    cmdController->commands["CMD_RequestWorld"] = &CMD_RequestWorld;
    cmdController->commands["CMD_MoveTo"] = &CMD_MoveTo;
    cmdController->commands["CMD_LookAt"] = &CMD_LookAt;
    cmdController->commands["CMD_DestroyBlock"] = &CMD_DestroyBlock;
    cmdController->commands["CMD_SetBlock"] = &CMD_SetBlock;
    cmdController->commands["CMD_RequestInventory"] = &CMD_RequestInventory;
}

void CMD_RequestWorld(const SyncVarTypeVariant& val, int connectId)
{
    printf("\nRequested world data\n");

    size_t worldDataLength;

    loadWorld("default", WorldData, worldDataLength);
    MsgData worldMsgData;
    worldMsgData.type = (uint)(MessageTypes::ClientRPC);

    RemoteFunctionCallData rmData;
    rmData.m_parameterType = (uint)SyncVarTypes::STRING;
    rmData.receiverId = connectId;
    rmData.parameterSize = worldDataLength;
    memcpy(rmData.m_methodName, "RPC_FetchWorld", strlen("RPC_FetchWorld"));

    uint chunkSize = 384;
    uint chunkCount = worldDataLength / chunkSize;
    uint remainderSize = worldDataLength % chunkSize;

    std::string a = WorldData;
    std::string firstHalf;
    std::string secondHalf;

    for (int i = 0; i < chunkCount; i++)
    {
        auto chunkData = a.substr(i*chunkSize, chunkSize);
        firstHalf = chunkData;
        memcpy(rmData.m_parameter, chunkData.c_str(), chunkSize);
        memcpy(worldMsgData.data, &rmData, sizeof(rmData));

        auto packet = enet_packet_create(&worldMsgData,sizeof(uint)+sizeof(RemoteFunctionCallData),ENET_PACKET_FLAG_RELIABLE);

        for(auto conn : connections)
        {
            if(conn.second->connectID != connectId)
                continue;
            enet_peer_send(conn.second,0,packet);
        }
    }

    if(remainderSize > 0)
    {
        memcpy(rmData.m_methodName, "RPC_FetchWorldLast", strlen("RPC_FetchWorldLast"));

        auto chunkData = a.substr(chunkCount * chunkSize, remainderSize);
        secondHalf = chunkData;

        memcpy(rmData.m_parameter, chunkData.c_str(), chunkSize);
        memcpy(worldMsgData.data, &rmData, sizeof(rmData));

        auto packet = enet_packet_create(&worldMsgData,sizeof(uint)+sizeof(RemoteFunctionCallData),ENET_PACKET_FLAG_RELIABLE);

        for(auto conn : connections)
        {
            if(conn.second->connectID != connectId)
                continue;
            enet_peer_send(conn.second,0,packet);
        }
    }
    printf("%s",firstHalf.c_str());
    printf("%s",secondHalf.c_str());
}

void CMD_RequestInventory(const SyncVarTypeVariant& val, int connectId)
{
    printf("\nRequested inventory data\n");

    size_t invDataLength;

    loadInventory("default", InventoryData, invDataLength);
    MsgData invMsgData;
    invMsgData.type = (uint)(MessageTypes::ClientRPC);

    RemoteFunctionCallData rmData;
    rmData.m_parameterType = (uint)SyncVarTypes::STRING;
    rmData.receiverId = connectId;
    rmData.parameterSize = invDataLength;
    memcpy(rmData.m_methodName, "RPC_FetchInventory", strlen("RPC_FetchInventory"));

    uint chunkSize = 384;
    uint chunkCount = invDataLength / chunkSize;
    uint remainderSize = invDataLength % chunkSize;

    std::string a = InventoryData;
    std::string firstHalf;
    std::string secondHalf;

    for (int i = 0; i < chunkCount; i++)
    {
        auto chunkData = a.substr(i*chunkSize, chunkSize);
        firstHalf = chunkData;
        memcpy(rmData.m_parameter, chunkData.c_str(), chunkSize);
        memcpy(invMsgData.data, &rmData, sizeof(rmData));

        auto packet = enet_packet_create(&invMsgData,sizeof(uint)+sizeof(RemoteFunctionCallData),ENET_PACKET_FLAG_RELIABLE);

        for(auto conn : connections)
        {
            if(conn.second->connectID != connectId)
                continue;
            enet_peer_send(conn.second,0,packet);
        }
    }

    if(remainderSize > 0)
    {
        memcpy(rmData.m_methodName, "RPC_FetchInventoryLast", strlen("RPC_FetchInventoryLast"));

        auto chunkData = a.substr(chunkCount * chunkSize, remainderSize);
        secondHalf = chunkData;

        memcpy(rmData.m_parameter, chunkData.c_str(), chunkSize);
        memcpy(invMsgData.data, &rmData, sizeof(rmData));

        auto packet = enet_packet_create(&invMsgData,sizeof(uint)+sizeof(RemoteFunctionCallData),ENET_PACKET_FLAG_RELIABLE);

        for(auto conn : connections)
        {
            if(conn.second->connectID != connectId)
                continue;
            enet_peer_send(conn.second,0,packet);
        }
    }
    printf("%s",firstHalf.c_str());
    printf("%s",secondHalf.c_str());
}

void CMD_MoveTo(const SyncVarTypeVariant& val, int connectId)
{
    glm::vec2 newPosCalculatedByOwner = std::get<glm::vec2>(val);

    MsgData msgData;
    msgData.type = (uint)(MessageTypes::ClientRPC);

    RemoteFunctionCallData rmData;
    rmData.m_parameterType = (uint)SyncVarTypes::VEC2;
    rmData.parameterSize = sizeof(glm::vec2);
    rmData.receiverId = connectId;
    memcpy(rmData.m_parameter, &newPosCalculatedByOwner, sizeof(newPosCalculatedByOwner));
    memcpy(rmData.m_methodName, "RPC_UpdatePlayerPosition", strlen("RPC_UpdatePlayerPosition"));

    memcpy(msgData.data, &rmData, sizeof(rmData));

    auto packet = enet_packet_create(&msgData,sizeof(uint)+sizeof(RemoteFunctionCallData),NULL);

    for(auto conn : connections)
    {
        if(conn.second->connectID == connectId)
            continue;
        enet_peer_send(conn.second,0,packet);
    }
}

void CMD_LookAt(const SyncVarTypeVariant& val, int connectId)
{
    int newFacingDirection = std::get<int>(val);

    MsgData msgData;
    msgData.type = (uint)(MessageTypes::ClientRPC);

    RemoteFunctionCallData rmData;
    rmData.m_parameterType = (uint)SyncVarTypes::INT;
    rmData.parameterSize = sizeof(int);
    rmData.receiverId = connectId;
    memcpy(rmData.m_parameter, &newFacingDirection, sizeof(newFacingDirection));
    memcpy(rmData.m_methodName, "RPC_UpdatePlayerFacingDirection", strlen("RPC_UpdatePlayerFacingDirection"));

    memcpy(msgData.data, &rmData, sizeof(rmData));

    auto packet = enet_packet_create(&msgData,sizeof(uint)+sizeof(RemoteFunctionCallData),NULL);

    for(auto conn : connections)
    {
        if(conn.second->connectID == connectId)
            continue;
        enet_peer_send(conn.second,0,packet);
    }
}

void CMD_DestroyBlock(const SyncVarTypeVariant& val, int connectId)
{
    glm::vec2 destroyedBlock = std::get<glm::vec2>(val);

    MsgData msgData;
    msgData.type = (uint)(MessageTypes::ClientRPC);

    RemoteFunctionCallData rmData;
    rmData.m_parameterType = (uint)SyncVarTypes::VEC2;
    rmData.parameterSize = sizeof(glm::vec2);
    rmData.receiverId = connectId;
    memcpy(rmData.m_parameter, &destroyedBlock, sizeof(destroyedBlock));
    memcpy(rmData.m_methodName, "RPC_OnBlockDestroyed", strlen("RPC_OnBlockDestroyed"));

    memcpy(msgData.data, &rmData, sizeof(rmData));

    auto packet = enet_packet_create(&msgData,sizeof(uint)+sizeof(RemoteFunctionCallData),NULL);

    for(auto conn : connections)
    {
        if(conn.second->connectID == connectId)
            continue;
        enet_peer_send(conn.second,0,packet);
    }
}

void CMD_SetBlock(const SyncVarTypeVariant& val, int connectId)
{
    printf("\nSet block\n");
    glm::vec3 msg = std::get<glm::vec3>(val);

    MsgData msgData;
    msgData.type = (uint)(MessageTypes::ClientRPC);

    RemoteFunctionCallData rmData;
    rmData.m_parameterType = (uint)SyncVarTypes::VEC3;
    rmData.parameterSize = sizeof(glm::vec2);
    rmData.receiverId = connectId;
    memcpy(rmData.m_parameter, &msg, sizeof(msg));
    memcpy(rmData.m_methodName, "RPC_OnBlockSet", strlen("RPC_OnBlockSet"));

    memcpy(msgData.data, &rmData, sizeof(rmData));

    auto packet = enet_packet_create(&msgData,sizeof(uint)+sizeof(RemoteFunctionCallData),NULL);

    for(auto conn : connections)
    {
        //if(conn.second->connectID == connectId)
            //continue;
        enet_peer_send(conn.second,0,packet);
    }
}