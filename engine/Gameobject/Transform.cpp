//
// Created by erhan güven on 17.09.2023.
//

#include "Transform.h"
#include "Rendering/ShaderManager.h"
#include "Physics/PhysicsConstants.h"
#include "Window/Window.h"

namespace Engine
{
    void Transform::update(float dt, bool hasRenderer)
    {
        if(m_isRigidBody)
        {
            float gravity = Engine::Physics::PhysicsConstants::gravityScale;
            if(rigidBody.velocity.y != 0)
            {
                jumpExtraMultiplier *= 1.055f;
            }
            rigidBody.velocity.y -= gravity * jumpExtraMultiplier;
            m_position.y += rigidBody.velocity.y * dt * .001f;
        }

        if(hasRenderer)
        {
            ShaderProgram* shaderProgram = Engine::ShaderManager::getInstance()->getProgram(EngineShaderPrograms::Default);
            auto m_sc = m_scale;
            m_sc.x *= (!mirror ? 1.0f : -1.0f) * 100.0f / Engine::Window::windowSize.x;
            m_sc.y *= 100.0f / Engine::Window::windowSize.y;
            shaderProgram->setUniform("a_pos", &m_position);
            shaderProgram->setUniform("a_scale", &m_sc);
        }
    }
} // Engine