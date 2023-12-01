//
// Created by erhan güven on 1.12.2023.
//

#include "Button.h"
#include "Scene/SceneManager.h"
#include <Input/InputHandler.h>

namespace Engine
{
    void Button::update(float dt)
    {
        ImageObject::update(dt);

        float mouseX = InputHandler::mousePosition.x/400.0f - 1.0f;
        float mouseY = -InputHandler::mousePosition.y/300.0f + 1.0f;
        float left = getTransform()->getPosition().x - getTransform()->getScale().x * .5f;
        float right = getTransform()->getPosition().x + getTransform()->getScale().x * .5f;
        float up = getTransform()->getPosition().y + getTransform()->getScale().y * .5f;
        float bottom = getTransform()->getPosition().y - getTransform()->getScale().y * .5f;

        if(mouseX > left && mouseX < right && mouseY < up && mouseY > bottom)
        {
            if(!InputHandler::isPressingMouseButton(GLFW_MOUSE_BUTTON_1))
            {
                btnState.hovering = true;
                btnState.clicked = false;
                glm::vec4 color = {1,1,1,.75f};
                getRenderer()->setColor(color);
            }
            else if(btnState.hovering && !btnState.clicked)
            {
                btnState.clicked = true;
                glm::vec4 color = glm::vec4{.75};
                getRenderer()->setColor(color);
                Engine::SceneManager::setCurrentScene(1);
            }
        }
        else if(btnState.hovering)
        {
            btnState.hovering = false;
            glm::vec4 color = {1,1,1,1.0f};
            getRenderer()->setColor(color);
        }
    }
} // Engine