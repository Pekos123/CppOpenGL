#include <Application.h>
#include <Renderer.h>
#include <VertexArray.h>
#include <Shader.h>
#include <Camera.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <filesystem>

class Sandbox : public e::Application
{
    std::shared_ptr<e::VertexArray> m_VertexArray;
    std::shared_ptr<e::VertexArray> m_SecVertexArray;

    std::shared_ptr<e::Shader> m_LightShader;
    std::shared_ptr<e::Shader> m_Shader;

    glm::vec3 color = { 0.2f, 0.3f, 0.8f };
    glm::vec3 lightColor = { 1.0f, 1.0f, 1.0f };
    glm::vec3 lightPos = { 0.1f, 0.7f, 0.3f };

    e::Camera camera;

    void DebugWindowRender()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::Begin("Debug Window");
                ImGui::InputFloat("Sensivity", &camera.sensivity, 0.1f, 0.5f);
                ImGui::ColorEdit3("Objects Color", (float*)&color);
                ImGui::InputFloat("Camera Speed", &camera.speed, 0.1f);
            ImGui::End();

            ImGui::Begin("Light soruce");
                ImGui::ColorEdit3("Light Color", (float*)&lightColor);
                ImGui::DragFloat3("Light Pos", glm::value_ptr(lightPos), 0.01f, -10.0f, 10.0f);
            ImGui::End();
        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void DebugWindowInit()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        
        ImGui_ImplGlfw_InitForOpenGL(m_Window->GetGLFWwindow(), true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }

    void DebugWindowShutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void SetShaders()
    {
        std::filesystem::path rootDir = std::filesystem::current_path().parent_path().parent_path().parent_path();
        std::string objVertex = (rootDir / "engine/shaders/obj/obj.vert").string();
        std::string objFragment = (rootDir / "engine/shaders/obj/obj.frag").string();
        std::string lightVertex = (rootDir / "engine/shaders/light/light.vert").string();
        std::string lightFragment = (rootDir / "engine/shaders/light/light.frag").string();

        m_Shader = std::make_shared<e::Shader>(e::Utils::ReadFile(objVertex), e::Utils::ReadFile(objFragment));
        m_LightShader = std::make_shared<e::Shader>(e::Utils::ReadFile(lightVertex), e::Utils::ReadFile(lightFragment));
    }

public:
    Sandbox() : camera(m_Window->GetWidth(), m_Window->GetHeight(), { 0.0f, 0.5f, 2.0f })
    {
        DebugWindowInit();

        #pragma region Buffers
        m_VertexArray = std::make_shared<e::VertexArray>();
        m_SecVertexArray = std::make_shared<e::VertexArray>();

        // Vertices with Normals: Position (3), Normal (3)
        // Duplicated for face-specific normals
        float vertices[] = {
            // PYRAMID (Approximate positions from previous code, duplicated for faces)
            // Front Face
            -0.7f, 0.0f,  0.2f,  0.0f, 0.447f, 0.894f,
            -0.3f, 0.0f,  0.2f,  0.0f, 0.447f, 0.894f,
            -0.5f, 0.4f,  0.0f,  0.0f, 0.447f, 0.894f,
            // Back Face
            -0.7f, 0.0f, -0.2f,  0.0f, 0.447f, -0.894f,
            -0.3f, 0.0f, -0.2f,  0.0f, 0.447f, -0.894f,
            -0.5f, 0.4f,  0.0f,  0.0f, 0.447f, -0.894f,
            // Left Face
            -0.7f, 0.0f,  0.2f, -0.894f, 0.447f, 0.0f,
            -0.7f, 0.0f, -0.2f, -0.894f, 0.447f, 0.0f,
            -0.5f, 0.4f,  0.0f, -0.894f, 0.447f, 0.0f,
            // Right Face
            -0.3f, 0.0f,  0.2f,  0.894f, 0.447f, 0.0f,
            -0.3f, 0.0f, -0.2f,  0.894f, 0.447f, 0.0f,
            -0.5f, 0.4f,  0.0f,  0.894f, 0.447f, 0.0f,
            // Bottom Face (2 triangles)
            -0.7f, 0.0f,  0.2f,  0.0f, -1.0f, 0.0f,
            -0.7f, 0.0f, -0.2f,  0.0f, -1.0f, 0.0f,
            -0.3f, 0.0f, -0.2f,  0.0f, -1.0f, 0.0f,
            -0.3f, 0.0f,  0.2f,  0.0f, -1.0f, 0.0f,

            // CUBE (Position + Normal)
            // Front face
            0.3f, 0.0f, 0.2f,  0.0f, 0.0f, 1.0f,
            0.7f, 0.0f, 0.2f,  0.0f, 0.0f, 1.0f,
            0.7f, 0.4f, 0.2f,  0.0f, 0.0f, 1.0f,
            0.3f, 0.4f, 0.2f,  0.0f, 0.0f, 1.0f,
            // Back face
            0.3f, 0.0f, -0.2f,  0.0f, 0.0f, -1.0f,
            0.7f, 0.0f, -0.2f,  0.0f, 0.0f, -1.0f,
            0.7f, 0.4f, -0.2f,  0.0f, 0.0f, -1.0f,
            0.3f, 0.4f, -0.2f,  0.0f, 0.0f, -1.0f,
            // Top face
            0.3f, 0.4f,  0.2f,  0.0f, 1.0f, 0.0f,
            0.7f, 0.4f,  0.2f,  0.0f, 1.0f, 0.0f,
            0.7f, 0.4f, -0.2f,  0.0f, 1.0f, 0.0f,
            0.3f, 0.4f, -0.2f,  0.0f, 1.0f, 0.0f,
            // Bottom face
            0.3f, 0.0f,  0.2f,  0.0f, -1.0f, 0.0f,
            0.7f, 0.0f,  0.2f,  0.0f, -1.0f, 0.0f,
            0.7f, 0.0f, -0.2f,  0.0f, -1.0f, 0.0f,
            0.3f, 0.0f, -0.2f,  0.0f, -1.0f, 0.0f,
            // Left face
            0.3f, 0.0f, -0.2f, -1.0f, 0.0f, 0.0f,
            0.3f, 0.0f,  0.2f, -1.0f, 0.0f, 0.0f,
            0.3f, 0.4f,  0.2f, -1.0f, 0.0f, 0.0f,
            0.3f, 0.4f, -0.2f, -1.0f, 0.0f, 0.0f,
            // Right face
            0.7f, 0.0f, -0.2f,  1.0f, 0.0f, 0.0f,
            0.7f, 0.0f,  0.2f,  1.0f, 0.0f, 0.0f,
            0.7f, 0.4f,  0.2f,  1.0f, 0.0f, 0.0f,
            0.7f, 0.4f, -0.2f,  1.0f, 0.0f, 0.0f
        };

        uint32_t indices[] = {
            // Pyramid
            0, 1, 2,  3, 4, 5,  6, 7, 8,  9, 10, 11,
            12, 13, 14, 12, 14, 15,
            // Cube (offset by 16 vertices)
            16, 17, 18, 16, 18, 19, // Front
            20, 21, 22, 20, 22, 23, // Back
            24, 25, 26, 24, 26, 27, // Top
            28, 29, 30, 28, 30, 31, // Bottom
            32, 33, 34, 32, 34, 35, // Left
            36, 37, 38, 36, 38, 39  // Right
        };

        // Light cube vertices (Only positions)
        float lightVertices[] = {
            -0.05f, -0.05f, -0.05f,
             0.05f, -0.05f, -0.05f,
             0.05f,  0.05f, -0.05f,
            -0.05f,  0.05f, -0.05f,
            -0.05f, -0.05f,  0.05f,
             0.05f, -0.05f,  0.05f,
             0.05f,  0.05f,  0.05f,
            -0.05f,  0.05f,  0.05f
        };

        uint32_t lightIndices[] = {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4,
            0, 4, 7, 7, 3, 0,
            1, 5, 6, 6, 2, 1,
            3, 7, 6, 6, 2, 3,
            0, 4, 5, 5, 1, 0
        };

        // Main objects
        auto vb = std::make_shared<e::VertexBuffer>(vertices, sizeof(vertices));
        vb->SetLayout({
            { e::ShaderDataType::Float3, "aPos" },
            { e::ShaderDataType::Float3, "aNormal" }
        });
        m_VertexArray->AddVertexBuffer(vb);
        auto ib = std::make_shared<e::IndexBuffer>(indices, sizeof(indices) / sizeof(uint32_t));
        m_VertexArray->SetIndexBuffer(ib); 

        // Light object
        auto lvb = std::make_shared<e::VertexBuffer>(lightVertices, sizeof(lightVertices));
        lvb->SetLayout({ { e::ShaderDataType::Float3, "aPos" } });
        m_SecVertexArray->AddVertexBuffer(lvb);
        auto lib = std::make_shared<e::IndexBuffer>(lightIndices, sizeof(lightIndices) / sizeof(uint32_t));
        m_SecVertexArray->SetIndexBuffer(lib);

        #pragma endregion Buffers
        
        SetShaders();
    }

    ~Sandbox()
    {
        DebugWindowShutdown();
    }

    void OnUpdate() override
    {
        e::Renderer::SetClearColor({ 0.1f, 0.1f, 0.15f, 1.0f });
        e::Renderer::Clear();

        glm::mat4 viewProj = camera.GetViewProjectionMatrix(45.0f, 0.1f, 100.0f);
        glm::mat4 model = glm::mat4(1.0f);

        m_Shader->Bind();
        m_Shader->SetUniformFloat3("objectColor", color); 
        m_Shader->SetUniformFloat3("lightColor", lightColor); 
        m_Shader->SetUniformFloat3("viewPos", camera.GetPosition()); 
        m_Shader->SetUniformFloat3("lightPos", lightPos); 
        m_Shader->SetUniformMat4("u_ViewProj", viewProj);
        m_Shader->SetUniformMat4("u_Model", model);
        
        e::Renderer::DrawIndexed(m_VertexArray);

        // Render light source at lightPos
        m_LightShader->Bind();
        glm::mat4 lightModel = glm::translate(glm::mat4(1.0f), lightPos);
        m_LightShader->SetUniformMat4("u_ViewProj", viewProj);
        m_LightShader->SetUniformMat4("u_Model", lightModel);
        m_LightShader->SetUniformFloat3("lightColor", lightColor);
        
        e::Renderer::DrawIndexed(m_SecVertexArray);

        camera.Inputs(m_Window.get());
        //camera.position = lightPos; // Move camera to light position for better view of the scene

        DebugWindowRender();
    }
};

int main()
{
    Sandbox* app = new Sandbox();
    app->Run();
    delete app;
    return 0;
}
