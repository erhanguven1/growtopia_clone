//
// Created by erhan güven on 17.09.2023.
//

#ifndef GROWTOPIA_CLONE_TRANSFORM_H
#define GROWTOPIA_CLONE_TRANSFORM_H

#include <glm/glm.hpp>

namespace Engine
{
    class Transform
    {
    public:
        void getTransformationValues(const glm::vec2* position = nullptr, const glm::vec2* scale = nullptr, const float* rotation = nullptr);
        void update();
        inline void addToPosition(glm::vec2& delta)
        {
            m_position += delta;
        }
    private:
        glm::vec2 m_position = glm::vec2(0.0f);
        glm::vec2 m_scale = glm::vec2(1.0f);
        float m_rotation = 0.0f;
    };

} // Engine

#endif //GROWTOPIA_CLONE_TRANSFORM_H
