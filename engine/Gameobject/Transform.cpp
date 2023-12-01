//
// Created by erhan güven on 17.09.2023.
//

#include "Transform.h"
#include "Rendering/ShaderManager.h"
#include "Physics/PhysicsConstants.h"
#include "Window/Window.h"
#include "bx/math.h"

namespace Engine
{
    void Transform::update(float dt, bool hasRenderer, bool isUi)
    {
        if(m_isRigidBody)
        {
            float gravity = Engine::Physics::PhysicsConstants::gravityScale;
            if(rigidBody.velocity.y != 0)
            {
                jumpExtraMultiplier *= 1.055f;
            }
            rigidBody.velocity.y -= gravity * jumpExtraMultiplier;
            m_position.y += rigidBody.velocity.y * dt * .5f;
        }

        if(hasRenderer)
        {
            ShaderProgram* shaderProgram;
            if(!isUi)
                shaderProgram = Engine::ShaderManager::getInstance()->getProgram(EngineShaderPrograms::Default);
            else
                shaderProgram = Engine::ShaderManager::getInstance()->getProgram(EngineShaderPrograms::DefaultUI);

            auto m_pos = glm::vec3(m_position.x - 400.0f, m_position.y - 300.0f, depth);
            auto m_sc = m_scale;
            m_sc.x *= (!mirror ? 1.0f : -1.0f) * 800.0f / Engine::Window::windowSize.x;
            m_sc.y *= 600.0f / Engine::Window::windowSize.y;
            shaderProgram->setUniform("a_pos", &m_pos);
            shaderProgram->setUniform("a_scale", &m_sc);
        }
    }
} // Engine