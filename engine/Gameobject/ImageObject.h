//
// Created by erhan güven on 16.09.2023.
//

#ifndef GROWTOPIA_CLONE_IMAGEOBJECT_H
#define GROWTOPIA_CLONE_IMAGEOBJECT_H

#include "Gameobject.h"

namespace Engine
{

    class ImageObject : public Gameobject
    {
    public:
        ImageObject() : ImageObject("",1,false) {}
        explicit ImageObject(uint layerIndex) : ImageObject("",layerIndex, false) {}
        explicit ImageObject(const char* imagePath) : ImageObject(imagePath,1, false) {}
        ImageObject(const char* imagePath, uint layerIndex, bool isUi = false);

        void start() override;
        void update(float) override;
    };

} // Engine

#endif //GROWTOPIA_CLONE_IMAGEOBJECT_H
