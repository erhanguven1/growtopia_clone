//
// Created by erhan güven on 17.09.2023.
//

#include "Transform.h"
#include "Rendering/ShaderManager.h"

namespace Engine
{
    void Transform::getTransformationValues(const glm::vec2 *position, const glm::vec2 *scale, const float *rotation)
    {
        if(position != nullptr)
            position = &m_position;
        if(scale != nullptr)
            scale = &m_scale;
        if(rotation != nullptr)
            rotation = &m_rotation;
    }

    void Transform::update()
    {
        ShaderProgram* shaderProgram = Engine::ShaderManager::getInstance()->getProgram(EngineShaderPrograms::Default);
        shaderProgram->setUniform("a_pos", &m_position);
        shaderProgram->setUniform("a_scale", &m_scale);
    }
} // Engine