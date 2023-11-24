//
// Created by erhan güven on 28.09.2023.
//

#ifndef GROWTOPIA_CLONE_NETWORKSTRUCTS_H
#define GROWTOPIA_CLONE_NETWORKSTRUCTS_H

#endif //GROWTOPIA_CLONE_NETWORKSTRUCTS_H

#include <enet/enet.h>
#include <variant>
#include <glm/glm.hpp>

using SyncVarTypeVariant = std::variant<int, float, glm::vec2>;

enum class MessageTypes
{
    PlayerData = 1,
    SyncVar,
    Command,
    ClientRPC
};

enum class SyncVarTypes
{
    INT,
    FLOAT,
    VEC2
};

struct MsgData
{
    uint type;
    enet_uint8 data[512] = {0};
};

struct PlayerData
{
    char m_name[32] = "";
    float m_posX;
    float m_posY;
};

struct SyncVarData
{
    uint m_id = -1;
    uint m_dataType;
    enet_uint8 data[512] = {0};
};

struct RemoteFunctionCallData
{
    char m_methodName[32] = "";
    uint receiverId = -1;
    uint m_parameterType;
    uint parameterSize;
    enet_uint8 m_parameter[64] = {0};
};