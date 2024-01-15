//
// Created by erhan güven on 13.11.2023.
//

#ifndef GROWTOPIA_CLONE_TILE_H
#define GROWTOPIA_CLONE_TILE_H

#include <glm/vec2.hpp>
#include <Gameobject/ImageObject.h>
#include <Common/BlockTypes.h>

namespace Game
{

    class Tile
    {
    public:
        Tile(const Tile&) = delete;
        Tile() = delete;
        explicit Tile(int i, int j, BlockType blockType, glm::ivec2& position);
        explicit Tile(int i, int j, glm::ivec2& position);

        void setBlockType(BlockType blockType, bool fromRpc = false);

        void takeDamage(float dmg);
        inline glm::vec2& getBlockPosition()
        {
            if(blockImage)
            {
                return blockImage->getTransform()->getPosition();
            }
        }
        inline BlockType getBlockType()
        {
            return m_BlockType;
        }
    private:
        glm::ivec2 m_Index;
        glm::ivec2 m_Position;
        BlockType m_BlockType;
        float m_Health = 100.0f;

        Engine::ImageObject* blockImage{};
        void setBlockImage();
    };

} // Game

#endif //GROWTOPIA_CLONE_TILE_H
