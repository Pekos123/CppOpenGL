#ifndef RENDERER_H
#define RENDERER_H

#pragma once
#include "VertexArray.h"
#include "Shader.h"

#include <map>

namespace e
{
    class Renderer
    {
    public:
        // hashmap where key is shader type (vertex, fragment) and value is the vertices and indices for that shader type
        // instead of every object to have their own vertex and index buffer, they will share the same buffers based on the shader type they use
        static std::map<e::Shader*, std::pair< std::vector<float>, std::vector<uint32_t>>> s_Buffers; // shader -> (vertices, indices)

    public:
        static void Clear();
        static void SetClearColor(const glm::vec4& color);
        
        static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray);
        static void DrawFromBuffers(e::Shader* shader);
    };
}
#endif // RENDERER_H