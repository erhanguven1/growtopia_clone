//
// Created by erhan güven on 15.01.2024.
//

#ifndef GROWTOPIA_CLONE_BLOCKTYPES_H
#define GROWTOPIA_CLONE_BLOCKTYPES_H

#include <string>

namespace Game
{
    enum class BlockType
    {
        Empty = 100,
        Dirt,
        CaveBackground
    };

    class BlockTypeHelper
    {
    public:
        inline static std::string getBlockFilename(const BlockType& blockType)
        {
            switch (blockType)
            {
                case BlockType::Empty:
                    return "";
                case BlockType::Dirt:
                    return "dirt_mid.png";
                case BlockType::CaveBackground:
                    return "cave_bg.png";
            }
        }
    };

} // Game

#endif //GROWTOPIA_CLONE_BLOCKTYPES_H
