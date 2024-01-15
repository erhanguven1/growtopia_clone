//
// Created by erhan güven on 13.11.2023.
//

#include "World.h"
#include <tinyxml2/tinyxml2.h>

namespace Game
{
    namespace DefaultWorldValues
    {
        constexpr int world_x = 16;
        constexpr int world_y = 8;
    }

    World::World()
    {
        for (int i = 0; i < DefaultWorldValues::world_x; ++i)
        {
            m_grid.emplace_back();
            for (int j = 0; j < DefaultWorldValues::world_y; ++j)
            {
                glm::ivec2 pos = {i,j};
                Tile* tile = new Tile(i,j,j <= 2 ? BlockType::Dirt : BlockType::Empty,pos);
                m_grid.back().push_back(tile);
            }
        }
    }

    World::World(std::vector<std::vector<Tile *>>& grid)
    {
        m_grid = grid;
    }

    World::World(const std::string& xmlWorldData)
    {
        using namespace tinyxml2;

        auto* doc = new XMLDocument();
        doc->Parse(xmlWorldData.c_str());

        auto* mapDataElement = doc->FirstChildElement("WorldData");
        auto* gridElement = mapDataElement->FirstChildElement();

        auto* tileElement = gridElement->FirstChildElement();

        while (tileElement != nullptr)
        {
            const char* tileType = tileElement->Attribute("Type");
            int i = atoi(tileElement->FirstChildElement("X")->GetText());
            int j = atoi(tileElement->FirstChildElement("Y")->GetText());
            uint blockType = atoi(tileType);

            if(i + 1 > m_grid.size())
            {
                m_grid.resize(i + 1);
            }
            if(j + 1 > m_grid[i].size())
            {
                m_grid[i].resize(j + 1);
            }
            glm::ivec2 pos = {(i-7.5f)*50,(j-5.5f)*50};

            m_grid[i][j] = new Tile(i,j,(BlockType)blockType,pos);

            tileElement = tileElement->NextSiblingElement();
        }
    }
} // Game