//
// Created by erhan güven on 13.11.2023.
//

#include "Tile.h"
#include "Scene/SceneManager.h"
#include "Networking/Client.h"

namespace Game
{
    Tile::Tile(BlockType blockType, glm::ivec2 &position) : m_BlockType(blockType), m_Position(position)
    {
        if(blockType != BlockType::Empty)
        {
            setBlockImage();
        }
    }
    Tile::Tile(glm::ivec2 &position) : Tile(BlockType::Empty, position) {}

    void Tile::setBlockType(BlockType blockType)
    {
        m_BlockType = blockType;
        if(blockType == BlockType::Empty)
        {
            if(blockImage)
            {
                glm::vec2 blockIndex = {m_Position.x, m_Position.y};
                Engine::Client::getInstance()->callCommand("CMD_DestroyBlock", blockIndex);
                blockImage->isDead = true;
                m_Health = 100;
            }
        }
        else
        {
            setBlockImage();
        }
    }

    void Tile::setBlockImage()
    {
        if(blockImage == nullptr)
        {
            blockImage = Engine::SceneManager::getCurrentScene()->spawn<Engine::ImageObject>("/Users/erhanguven/CLionProjects/growtopia_clone/growtopia_clone/Resources/dirt_mid.png",1);
            blockImage->getTransform()->setPositionX(m_Position.x/8.0f-.9375f);
            blockImage->getTransform()->setPositionY(m_Position.y/6.06f-1.0f+.165f/2.0f);
            blockImage->getTransform()->hasCollider = true;
        }
        else
        {
            blockImage->getRenderer()->setTexture("/Users/erhanguven/CLionProjects/growtopia_clone/growtopia_clone/Resources/cave_bg.png");
        }
    }

    void Tile::takeDamage(float dmg)
    {
        if(m_BlockType == BlockType::Empty)
            return;

        m_Health -= dmg * (m_BlockType == BlockType::CaveBackground ? .25f : 1.0f);
        printf("Health: %f\n", m_Health);
        glm::vec4 c = {1.0f, m_Health/100.0f, m_Health/100.0f, 1.0f};
        blockImage->getRenderer()->setColor(c);
        if(m_Health <= 0)
        {
            if(m_BlockType == BlockType::CaveBackground)
                setBlockType(BlockType::Empty);
            if(m_BlockType == BlockType::Dirt)
            {
                setBlockType(BlockType::CaveBackground);
                c = glm::vec4(1.0f);
                blockImage->getRenderer()->setColor(c);
                m_Health = 100;
                blockImage->getTransform()->hasCollider = false;
            }
        }
    }
} // Game