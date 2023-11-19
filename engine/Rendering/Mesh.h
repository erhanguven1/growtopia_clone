//
// Created by erhan güven on 30.08.2023.
//

#ifndef GROWTOPIA_CLONE_MESH_H
#define GROWTOPIA_CLONE_MESH_H

#include <bgfx/bgfx.h>
#include <stb_image.h>

namespace Engine
{
    // Vertex data for the triangle
    struct PosColorVertex
    {
        float x, y;
        uint32_t abgr;
        int16_t tex_x, text_y;
    };

    class Mesh
    {
    public:
        void init();
        void init(const char* imagePath);
        void bindVertexAndIndexBuffer();
        void setTexture(const char* path);

    private:
        bgfx::VertexLayout vertexLayout;
        bgfx::VertexBufferHandle vertexBuffer;
        bgfx::IndexBufferHandle indexBuffer;
        bgfx::TextureHandle textureHandle;

        PosColorVertex vertices[4] = {
                { -0.5f, -0.5f, 0xFFFFFFFF, 0,0 }, // Vertex 0: Position (-0.5, -0.5, 0), Color Blue
                { 0.5f, -0.5f, 0xFFFFFFFF,0x7fff,0 },  // Vertex 1: Position (0.5, -0.5, 0), Color Green
                { 0.5f, 0.5f, 0xFFFFFFFF,0x7fff,0x7fff },   // Vertex 2: Position (0.5, 0.5, 0), Color Red
                { -0.5f, 0.5f, 0xFFFFFFFF,0,0x7fff }   // Vertex 2: Position (-0.5, 0.5, 0), Color Red
        };

        const uint16_t kTriangleIndices[6] =
                {
                        0, 1, 2,
                        0, 2, 3,
                };
        stbi_uc* data;
    };

} // Engine

#endif //GROWTOPIA_CLONE_MESH_H
