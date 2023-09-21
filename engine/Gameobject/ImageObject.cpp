//
// Created by erhan güven on 16.09.2023.
//

#include "ImageObject.h"

namespace Engine
{
    ImageObject::ImageObject(const char *imagePath, const uint layerIndex)  : Gameobject(layerIndex)
    {
        addRenderer();
    }

    void ImageObject::start()
    {

    }

    void ImageObject::update(float)
    {

    }
} // Engine