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
        inline Tile& getTileAt(int i, int j)
        {
            assert(grid.size() - 1 >= i);
            assert(grid[i].size() - 1 >= j);

            return *grid[i][j];
        }
        inline std::vector<std::vector<Tile*>> getGrid() const {return grid;}
    private:
        std::vector<std::vector<Tile*>> grid;
    };

} // Game

#endif //GROWTOPIA_CLONE_WORLD_H
