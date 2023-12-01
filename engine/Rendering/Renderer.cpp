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
        mesh = std::make_unique<Mesh>();
        mesh->init();
    }
    Renderer::Renderer(const char *imagePath, bool isUi)
    {
        mesh = std::make_unique<Mesh>(isUi);
        mesh->init(imagePath);
    }
    void Renderer::render()
    {
        assert(mesh != nullptr);

        mesh->setColor(m_Color);
        mesh->bindVertexAndIndexBuffer();
        /*bgfx::setState
        (0
        | BGFX_STATE_WRITE_RGB
        | BGFX_STATE_WRITE_A
        | BGFX_STATE_BLEND_ALPHA
        );
        bgfx::submit(0, Engine::ShaderManager::getInstance()->getProgram(Engine::EngineShaderPrograms::Default)->getProgramHandle());*/
    }

    void Renderer::setTexture(const char* imagePath)
    {
        mesh->setTexture(imagePath);
    }
} // Engine