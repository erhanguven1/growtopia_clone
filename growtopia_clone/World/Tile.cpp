//
// Created by erhan güven on 13.11.2023.
//

#include "Tile.h"
#include "Scene/SceneManager.h"
#include "Networking/Client.h"

namespace Game
{
    Tile::Tile(int i, int j, BlockType blockType, glm::ivec2 &position) : m_BlockType(blockType), m_Position(position), m_Index(i,j)
    {
        if(blockType != BlockType::Empty)
        {
            setBlockImage();

            if(blockType == BlockType::CaveBackground)
                blockImage->getTransform()->hasCollider = false;
        }
    }
    Tile::Tile(int i, int j, glm::ivec2 &position) : Tile(i, j, BlockType::Empty, position) {}

    void Tile::setBlockType(BlockType blockType, bool fromRpc)
    {
        m_BlockType = blockType;
        if(!fromRpc)
        {
            glm::vec2 blockIndex = {m_Index.x, m_Index.y};
            Engine::Client::getInstance()->callCommand("CMD_DestroyBlock", blockIndex);
        }

        if(blockType == BlockType::Empty)
        {
            if(blockImage)
            {
                blockImage->isDead = true;
                m_Health = 100;
            }
        }
        else
        {
            setBlockImage();

            if(blockType == BlockType::CaveBackground)
                blockImage->getTransform()->hasCollider = false;
            else
                blockImage->getTransform()->hasCollider = true;
        }
    }

    void Tile::setBlockImage()
    {
        std::string imageResourcesPath = "/Users/erhanguven/CLionProjects/growtopia_clone/growtopia_clone/Resources/";
        std::string imagePath = imageResourcesPath + (m_BlockType == BlockType::Dirt ? "dirt_mid.png" : "cave_bg.png");

        if(blockImage == nullptr || blockImage->isDead)
        {
            blockImage = Engine::SceneManager::getCurrentScene()->spawn<Engine::ImageObject>(imagePath.c_str(),1);
            blockImage->getTransform()->setPositionX(m_Position.x);
            blockImage->getTransform()->setPositionY(m_Position.y);
            blockImage->getTransform()->setScaleX(50);
            blockImage->getTransform()->setScaleY(50);
            blockImage->getTransform()->hasCollider = true;
        }
        else
        {
            blockImage->getRenderer()->setTexture(imagePath.c_str());
        }
    }

    void Tile::takeDamage(float dmg)
    {
        if(m_BlockType == BlockType::Empty)
            return;

        m_Health -= dmg * (m_BlockType == BlockType::CaveBackground ? 1.0f : 2.0f);
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
            }
        }
    }
} // Game