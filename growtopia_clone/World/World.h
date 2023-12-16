//
// Created by erhan güven on 13.11.2023.
//

#ifndef GROWTOPIA_CLONE_WORLD_H
#define GROWTOPIA_CLONE_WORLD_H

#include "Tile.h"
#include <vector>

namespace Game
{

    class World
    {
    public:
        World();
        World(std::vector<std::vector<Tile*>>& grid);
        World(const std::string& xmlWorldData);
        inline Tile& getTileAt(int i, int j)
        {
            assert(m_grid.size() - 1 >= i);
            assert(m_grid[i].size() - 1 >= j);

            return *m_grid[i][j];
        }
        inline std::vector<std::vector<Tile*>> getGrid() const {return m_grid;}
    private:
        std::vector<std::vector<Tile*>> m_grid;
    };

} // Game

#endif //GROWTOPIA_CLONE_WORLD_H
