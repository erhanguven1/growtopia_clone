//
// Created by erhan güven on 25.11.2023.
//

#ifndef GROWTOPIA_CLONE_CAMERA_H
#define GROWTOPIA_CLONE_CAMERA_H

#include <glm/vec2.hpp>

namespace Engine
{
    class Camera
    {
    public:
        inline void setZoom(float amount){m_zoomLevel = amount;}
        inline void changeZoom(float amount){m_zoomLevel += amount;}
        inline static Camera* getDefault()
        {
            if(defaultCamera == nullptr)
                defaultCamera = new Camera;

            return defaultCamera;
        }
        inline glm::vec2 getPosition(){return m_position;}
        inline glm::vec2 setPosition(glm::vec2 pos){m_position = pos;}
        inline float getZoom() const {return m_zoomLevel;}
    private:
        Camera(){}
        inline static Camera* defaultCamera = nullptr;
        glm::vec2 m_position{0.0f,0.0f};
        float m_zoomLevel = 1.0f;
    };

} // Engine

#endif //GROWTOPIA_CLONE_CAMERA_H
