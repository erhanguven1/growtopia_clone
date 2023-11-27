//
// Created by erhan güven on 13.11.2023.
//

#include "World.h"

namespace Game
{
    World::World()
    {
        for (int i = 0; i < 32; ++i) {
            grid.emplace_back();
            for (int j = 0; j < 32; ++j) {
                glm::ivec2 pos = {i,j};
                Tile* tile = new Tile(i,j,pos);
                grid.back().push_back(tile);
            }
        }
    }
} // Game