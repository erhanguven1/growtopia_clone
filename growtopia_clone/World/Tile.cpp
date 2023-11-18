//
// Created by erhan güven on 13.11.2023.
//

#include "Tile.h"

namespace Game
{
    Tile::Tile(BlockType blockType, glm::vec2 &position)
    {

    }
    Tile::Tile(glm::vec2 &position) : Tile(BlockType::Empty, position)
    {

    }

    void Tile::setBlockType(BlockType blockType)
    {

    }
} // Game