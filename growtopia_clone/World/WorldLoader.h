//
// Created by erhan güven on 27.11.2023.
//

#ifndef GROWTOPIA_CLONE_WORLDLOADER_H
#define GROWTOPIA_CLONE_WORLDLOADER_H

#include "World.h"

namespace Game
{

    class WorldLoader
    {
    public:
        static void loadWorld(const char* worldName, World* worldRef);
        static void saveWorld(const char* worldName, const World* worldRef);
    private:
        static FILE* fp;
    };

} // Game

#endif //GROWTOPIA_CLONE_WORLDLOADER_H
