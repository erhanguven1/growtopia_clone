//
// Created by erhan güven on 3.09.2023.
//

#include "Gameobject.h"

namespace Engine
{

    Gameobject::Gameobject(uint layerIndex) : layer(layerIndex)
    {
        transform = std::make_unique<Engine::Transform>();
    }
} // Engine