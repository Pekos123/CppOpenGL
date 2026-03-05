# Engine API Reference

A quick reference for the core classes available in the `e` namespace.

---

## 🛠 `e::Application`
The base class for your application. Inherit from this to create your own project.

### Methods
- `virtual void OnUpdate()`: Override this to implement your per-frame logic.
- `void Run()`: Starts the main loop.
- `bool ShouldClose() const`: Checks if the window should close.

---

## 🖌 `e::Renderer`
A static class for handling global rendering tasks.

### Methods
- `static void Clear()`: Clears the current color and depth buffers.
- `static void SetClearColor(const glm::vec4& color)`: Sets the background color.
- `static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)`: Executes a draw call for the given vertex array.

---

## 🏗 `e::VertexArray`
Wraps an OpenGL VAO (Vertex Array Object).

### Methods
- `void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)`: Adds a VBO and sets its layout.
- `void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)`: Sets the EBO/IBO for this vertex array.
- `void Bind()` / `void Unbind()`: Manages the OpenGL binding state.

---

## 📜 `e::Shader`
Wraps an OpenGL shader program.

### Methods
- `Shader(const std::string& vertexSource, const std::string& fragmentSource)`: Compiles and links the shader.
- `void Bind()` / `void Unbind()`: Activates the shader program.
- `void SetUniformMat4(const std::string& name, const glm::mat4& value)`: Sets a 4x4 matrix uniform.
- `void SetUniformFloat3(const std::string& name, const glm::vec3& value)`: Sets a vec3 uniform.

---

## 📷 `e::Camera`
Manages the view and projection matrices.

### Methods
- `Camera(int width, int height, glm::vec3 position)`: Initializes the camera.
- `glm::mat4 GetViewProjectionMatrix(...)`: Calculates and returns the combined VP matrix.
- `void Inputs(Window* window)`: Processes keyboard and mouse input to move the camera.

---

## 🛠 `e::Utils`
Static helper functions.

### Methods
- `static std::string ReadFile(const std::string& filePath)`: Reads a text file (useful for shaders).
