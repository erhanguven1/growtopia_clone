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
            m_position.y += rigidBody.velocity.y * dt * .5f;
        }

        if(hasRenderer)
        {
            ShaderProgram* shaderProgram = Engine::ShaderManager::getInstance()->getProgram(EngineShaderPrograms::Default);
            auto m_pos = glm::vec3(m_position.x - 400.0f, m_position.y - 300.0f, depth);
            auto m_sc = m_scale;
            m_sc.x *= (!mirror ? 1.0f : -1.0f) * 800.0f / Engine::Window::windowSize.x;
            m_sc.y *= 600.0f / Engine::Window::windowSize.y;
            shaderProgram->setUniform("a_pos", &m_pos);
            shaderProgram->setUniform("a_scale", &m_sc);

            float vertices[8]=
            {
                    -.5f*m_sc.x+m_pos.x,-.5f*m_sc.y+m_pos.y,
                    .5f*m_sc.x+m_pos.x,-.5f*m_sc.y+m_pos.y,
                    -.5f*m_sc.x+m_pos.x,.5f*m_sc.y+m_pos.y,
                    .5f*m_sc.x+m_pos.x,.5f*m_sc.y+m_pos.y
            };

            float topRightVertex[4] = {vertices[6],vertices[7],0,1.0f};
            float topLeftVertex[4] = {vertices[4],vertices[5],0,1.0f};

            float proj[16];
            float view[16];
            bx::mtxOrtho(proj, -400.0f, 400.0f, -300.0f,300.0f, 0.0f, 100.0f,0.0f, bgfx::getCaps()->homogeneousDepth);
            bx::mtxIdentity(view);

            float proj_x_view[16];
            bx::mtxMul(proj_x_view, proj, view);

            float resTopRight[4];
            float resTopLeft[4];

            bx::vec4MulMtx(resTopRight,topRightVertex,proj);
            bx::vec4MulMtx(resTopLeft,topLeftVertex,proj);
        }
    }
} // Engine