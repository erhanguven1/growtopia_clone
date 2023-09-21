//
// Created by erhan güven on 30.08.2023.
//

#ifndef GROWTOPIA_CLONE_MESH_H
#define GROWTOPIA_CLONE_MESH_H

#include <bgfx/bgfx.h>

namespace Engine
{
    // Vertex data for the triangle
    struct PosColorVertex
    {
        float x, y;
        uint32_t abgr;
    };

    class Mesh
    {
    public:
        void init();
        void bindVertexAndIndexBuffer();

    private:
        bgfx::VertexLayout vertexLayout;
        bgfx::VertexBufferHandle vertexBuffer;
        bgfx::IndexBufferHandle indexBuffer;

        PosColorVertex vertices[4] = {
                { -0.5f, -0.5f, 0xFFFFFFFF }, // Vertex 0: Position (-0.5, -0.5, 0), Color Blue
                { 0.5f, -0.5f, 0xFFFFFFFF },  // Vertex 1: Position (0.5, -0.5, 0), Color Green
                { 0.5f, 0.5f, 0xFFFFFFFF },   // Vertex 2: Position (0.5, 0.5, 0), Color Red
                { -0.5f, 0.5f, 0xFFFFFFFF }   // Vertex 2: Position (-0.5, 0.5, 0), Color Red
        };

        const uint16_t kTriangleIndices[6] =
                {
                        0, 1, 2,
                        0, 2, 3,
                };
    };

} // Engine

#endif //GROWTOPIA_CLONE_MESH_H
