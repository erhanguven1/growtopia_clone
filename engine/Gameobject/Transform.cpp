//
// Created by erhan güven on 17.09.2023.
//

#include "Transform.h"
#include "Rendering/ShaderManager.h"
#include "Physics/PhysicsConstants.h"

namespace Engine
{
    void Transform::update(float dt, bool hasRenderer)
    {
        if(m_isRigidBody)
        {
            const float gravity = Engine::Physics::PhysicsConstants::gravityScale;
            printf("%f",rigidBody.velocity.y);
            if(rigidBody.velocity.y > 0)
            {
                rigidBody.velocity.y -= gravity;
            }
            else
            {
                rigidBody.velocity.y -= gravity * 5.0f;
            }
            m_position.y += rigidBody.velocity.y * dt * .001f;
        }

        if(hasRenderer)
        {
            ShaderProgram* shaderProgram = Engine::ShaderManager::getInstance()->getProgram(EngineShaderPrograms::Default);
            shaderProgram->setUniform("a_pos", &m_position);
            shaderProgram->setUniform("a_scale", &m_scale);
        }
    }
} // Engine