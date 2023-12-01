//
// Created by erhan güven on 30.08.2023.
//

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "Mesh.h"
#include "ShaderManager.h"
#define STB_IMAGE_IMPLEMENTATION

namespace Engine
{
    void Mesh::init()
    {
        init(nullptr);
    }

    void Mesh::init(const char *imagePath)
    {
        vertexLayout.begin()
                .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
                .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Int16, true, true)
                .end();
        // Create vertex buffer
        vertexBuffer = bgfx::createVertexBuffer(
                bgfx::makeRef(vertices, sizeof(vertices)),vertexLayout
        );

        indexBuffer = bgfx::createIndexBuffer(bgfx::makeRef(kTriangleIndices, sizeof(kTriangleIndices)));

        setTexture(imagePath);
    }

    void Mesh::bindVertexAndIndexBuffer()
    {
        bgfx::setVertexBuffer(0, vertexBuffer);
        bgfx::setIndexBuffer(indexBuffer);

        ShaderProgram* shaderProgram = !isUi ? ShaderManager::getInstance()->getProgram(EngineShaderPrograms::Default) : ShaderManager::getInstance()->getProgram(EngineShaderPrograms::DefaultUI);

        shaderProgram->setUniform("a_color", &m_Color);

        if(textureHandle.idx != 0 && bgfx::isValid(textureHandle))
        {
            bgfx::UniformHandle* s_tex = shaderProgram->getUniform("s_tex");
            bgfx::setTexture(0, *s_tex, textureHandle);
        }

        bgfx::setState
                (0
                 | BGFX_STATE_WRITE_RGB
                 | BGFX_STATE_WRITE_A
                 | BGFX_STATE_BLEND_ALPHA
                );
        bgfx::submit(0, shaderProgram->getProgramHandle());

    }

    void Mesh::setColor(glm::vec4 color)
    {
        m_Color = color;
    }

    void Mesh::setTexture(const char *path)
    {
        if(path == nullptr)
        {
            path = "/Users/erhanguven/CLionProjects/growtopia_clone/growtopia_clone/Resources/default.png";
        }

        stbi_set_flip_vertically_on_load(true);
        int width, height, channels;
        data = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);

        const bgfx::Memory* mem = bgfx::makeRef(data, width * height * 4); // Assuming RGBA format

        if(textureHandle.idx == 0)
        {
            textureHandle = bgfx::createTexture2D(uint16_t(width), uint16_t(height), false, 1, bgfx::TextureFormat::RGBA8, 0, mem);
        }
        else
        {
            bgfx::destroy(textureHandle);
            textureHandle = bgfx::createTexture2D(uint16_t(width), uint16_t(height), false, 1, bgfx::TextureFormat::RGBA8, 0, mem);
        }
    }
} // Engine