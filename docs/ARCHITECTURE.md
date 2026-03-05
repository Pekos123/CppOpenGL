# Architecture Overview

This document describes the design and internal structure of the `GameEngine`.

## 🏛 High-Level Structure

The project is divided into two main layers:
1. **Engine Layer (`engine/`)**: A static library that encapsulates OpenGL logic and providing a high-level API for rendering.
2. **Client Layer (`sandbox/`)**: An executable that uses the engine to create a graphical application.

## 🔄 The Application Lifecycle

The engine uses a standard "Application-Loop" pattern:

1. **Inheritance**: The client creates a class that inherits from `e::Application`.
2. **Initialization**: The `e::Application` constructor initializes the `Window` (using GLFW and GLAD).
3. **Loop**: The `Run()` method contains the main engine loop, which calls `OnUpdate()` in every frame.
4. **Shutdown**: Resources are cleaned up in the destructors of the various classes.

## 🎨 Rendering Pipeline

### `e::Renderer`
A static class responsible for global rendering state and execution.
- **Clear & Colors**: Manages background clear color and buffer clearing.
- **DrawIndexed**: Takes a `VertexArray` and executes the OpenGL draw call.

### Abstraction Layer
The engine wraps low-level OpenGL objects into clean C++ classes:
- **`Buffer`**: Wraps `VBO` (Vertex Buffer Object) and `EBO` (Element Buffer Object). Supports data layouts for vertex attributes.
- **`VertexArray`**: Wraps `VAO` (Vertex Array Object), managing the relationship between buffers and attribute pointers.
- **`Shader`**: Manages the compilation, linking, and uniform setting of GLSL shaders.

## 🎥 Camera System
The `e::Camera` class handles the view and projection matrices. It supports:
- **Input Handling**: Processes keyboard (WASD) and mouse movement to update the camera's transform.
- **Math**: Uses GLM to calculate the View-Projection matrix required for vertex shaders.

## 🔧 Utilities
`e::Utils` provides common helper functions, such as `ReadFile`, which is used to load shader source code from disk.
