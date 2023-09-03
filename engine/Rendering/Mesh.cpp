//
// Created by erhan güven on 30.08.2023.
//

#include "Mesh.h"

namespace Engine
{
    void Mesh::init()
    {
        vertexLayout.begin()
                .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
                .end();
        // Create vertex buffer
        vertexBuffer = bgfx::createVertexBuffer(
                bgfx::makeRef(vertices, sizeof(vertices)),vertexLayout
        );

        indexBuffer = bgfx::createIndexBuffer(bgfx::makeRef(kTriangleIndices, sizeof(kTriangleIndices)));
    }

    void Mesh::bindVertexAndIndexBuffer()
    {
        bgfx::setVertexBuffer(0, vertexBuffer);
        bgfx::setIndexBuffer(indexBuffer);
    }
} // Engine