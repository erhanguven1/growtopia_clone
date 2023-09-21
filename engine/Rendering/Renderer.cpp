//
// Created by erhan güven on 30.08.2023.
//

#include <glm/vec2.hpp>
#include "Renderer.h"
#include "ShaderManager.h"

namespace Engine
{
    Renderer::Renderer()
    {
        mesh = new Mesh();
        mesh->init();
    }
    void Renderer::render()
    {
        if(mesh == nullptr)
            throw std::runtime_error("Mesh is NULL!");

        bgfx::setState(
                BGFX_STATE_WRITE_R
                | BGFX_STATE_WRITE_G
                | BGFX_STATE_WRITE_B
                | BGFX_STATE_WRITE_A
        );
        mesh->bindVertexAndIndexBuffer();
        bgfx::submit(0, Engine::ShaderManager::getInstance()->getProgram(Engine::EngineShaderPrograms::Default)->getProgramHandle());
    }
} // Engine