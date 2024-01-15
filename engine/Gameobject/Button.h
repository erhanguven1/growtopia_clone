//
// Created by erhan güven on 1.12.2023.
//

#ifndef GROWTOPIA_CLONE_BUTTON_H
#define GROWTOPIA_CLONE_BUTTON_H

#include "ImageObject.h"

namespace Engine
{
    struct ButtonState
    {
    public:
        bool hovering = false;
        bool clicked = false;
    };

    class Button : public ImageObject
    {
    public:
        explicit Button(const char* imagePath) : ImageObject(imagePath,1, true) {}
        explicit Button(const char* imagePath, uint layerIndex) : ImageObject(imagePath,layerIndex, true) {}
        void update(float) override;
        inline ButtonState* getButtonState(){return &btnState;}
        inline void registerOnClick(std::function<void()>& onClick){ m_onClick = onClick; }
    private:
        ButtonState btnState;
        std::function<void()> m_onClick;
    };

} // Engine

#endif //GROWTOPIA_CLONE_BUTTON_H
