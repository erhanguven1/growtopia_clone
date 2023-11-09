/* ./server/main.c */

#include <cstdio>
#include <enet/enet.h>
#include <cstring>
#include <Networking/NetworkStructs.h>
#include <unordered_map>
#include <vector>
#include <string>

void onReceivePacket(const ENetEvent& event);
void registerRpcMethodsToCommands();
void CMD_MoveTo(const SyncVarTypeVariant&, int);

std::unordered_map<enet_uint32 , ENetPeer*> connections;
std::vector<SyncVarTypeVariant> variables;

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

    ENetEvent event;
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

    // gameloop
    while(true)
    {
        ENetEvent event;
        /* Wait up to 1000 milliseconds for an event. */
        while (enet_host_service (server, &event, 1000) > 0)
        {
            switch (event.type)
            {
                case ENET_EVENT_TYPE_CONNECT:
                {
                    printf ("A new client connected from %x:%u.\n",
                            event.peer -> address.host,
                            event.peer -> address.port);

                    connections[event.peer->connectID] = event.peer;
                    printf("Connection ID: %u\n",event.peer->connectID);
                    break;
                }
                case ENET_EVENT_TYPE_RECEIVE:
                    printf ("A packet of length %u containing %s was received from %s on channel %u.\n",
                            event.packet -> dataLength,
                            event.packet -> data,
                            event.peer -> data,
                            event.channelID);

                    printf("testing");

                    onReceivePacket(event);
                    /* Clean up the packet now that we're done using it. */
                    enet_packet_destroy (event.packet);
                    break;

                case ENET_EVENT_TYPE_DISCONNECT:
                    printf ("%s disconnected.\n", event.peer -> data);
                    /* Reset the peer's client information. */
                    event.peer -> data = NULL;
            }
        }
    }

    enet_host_destroy(server);

    return 0;
}

void onReceivePacket(const ENetEvent& event)
{
    printf("testing");
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
                    glm::vec2 param;
                    memcpy(&param, rmCallData.m_parameter, sizeof(glm::vec2));
                    cmdController->commands.at(rmCallData.m_methodName)(param, event.peer->connectID);
                    break;
            }

            break;
        }
        case MessageTypes::ClientRPC:
            fprintf(stderr, "Server cannot get ClientRPC messages!\n");
            break;
    }
}

void registerRpcMethodsToCommands()
{
    cmdController->commands["CMD_MoveTo"] = &CMD_MoveTo;
}

void CMD_MoveTo(const SyncVarTypeVariant& val, int connectId)
{
    glm::vec2 direction = std::get<glm::vec2>(val);
    printf("Received command! Message: %d", direction);

    MsgData msgData;
    msgData.type = (uint)(MessageTypes::ClientRPC);

    RemoteFunctionCallData rmData;
    rmData.m_parameterType = (uint)SyncVarTypes::VEC2;
    rmData.receiverId = connectId;
    memcpy(rmData.m_parameter, &direction, sizeof(direction));
    memcpy(rmData.m_methodName, "RPC_UpdatePlayerPosition", strlen("RPC_UpdatePlayerPosition"));

    memcpy(msgData.data, &rmData, sizeof(rmData));

    auto packet = enet_packet_create(&msgData,sizeof(uint)+sizeof(RemoteFunctionCallData),NULL);

    enet_host_broadcast(server, 0, packet);
}