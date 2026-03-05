#include "Renderer.h"
#include <glad/glad.h>

std::map<e::Shader*, std::pair< std::vector<float>, std::vector<uint32_t>>> e::Renderer::s_Buffers;

namespace e
{
    void Renderer::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::SetClearColor(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void Renderer::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
    {
        vertexArray->Bind();
        glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    }
    // will render objects based on shader type, it will be used for objects that share the same shader and thus can share the same vertex and index buffers
    void Renderer::DrawFromBuffers(e::Shader* shader)
    {    
        auto& bufferData = Renderer::s_Buffers[shader];
        auto& vertices = bufferData.first;
        auto& indices = bufferData.second;

        // Create and bind vertex array
        uint32_t vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Create and bind vertex buffer
        uint32_t vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        // Create and bind index buffer
        uint32_t ibo;
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

        // Set vertex attribute pointers (assuming position only for simplicity)
        // change it to match your vertex format (e.g., if you have normals, texcoords, etc.)
        
        /*switch(e::BufferLayout)
        {
            case e::BufferLayout::Position:
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // Position
                break;
            case e::BufferLayout::PositionNormal:*/
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // Position
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // Normal
                //break;
        //}

        // Draw the object
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

        // Cleanup
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);
        glDeleteVertexArrays(1, &vao);
        delete shader; // Assuming ownership of shader, adjust if not
    }
}