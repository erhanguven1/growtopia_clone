//
// Created by erhan güven on 13.11.2023.
//

#ifndef GROWTOPIA_CLONE_TILE_H
#define GROWTOPIA_CLONE_TILE_H

#include <glm/vec2.hpp>

namespace Game
{
    enum class BlockType
    {
        Empty = 100,
        Dirt,
        CaveBackground
    };

    class Tile
    {
    public:
        Tile(const Tile&) = delete;
        Tile() = delete;
        explicit Tile(BlockType blockType, glm::vec2& position);
        explicit Tile(glm::vec2& position);

        void setBlockType(BlockType blockType);
    private:
        glm::vec2 m_Position;
        BlockType m_BlockType;
    };

} // Game

#endif //GROWTOPIA_CLONE_TILE_H
