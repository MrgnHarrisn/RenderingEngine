#include "Renderer/Renderer.h"
#include "Core/Window.h"
#include "Utils/Scene.h"
#include <glad/glad.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

Renderer::Renderer(Window* target)
    : m_viewMatrix(glm::mat4(1.0f))
    , m_projectionMatrix(glm::mat4(1.0f))
{
    attach(target);
    m_defaultShader.LoadFromFile("assets/Shaders/main.vert", "assets/Shaders/main.frag");
}

Renderer::~Renderer()
{
    // Cleanup if needed
    m_target = nullptr;
}

//void Renderer::setViewport(int width, int height)
//{
//    glViewport(0, 0, width, height);
//}

void Renderer::attach(Window* target)
{
    m_target = target;

    if (m_target) {
        // Make the window's context current
        m_target->makeContextCurrent();

        // Initialize GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            return;
        }

        // Setup OpenGL state
        setupOpenGLState();

        m_initialized = true;

        std::cout << "Renderer initialized with OpenGL " << glGetString(GL_VERSION) << std::endl;
    }
}

bool Renderer::init()
{
    if (!m_target || !m_initialized) {
        std::cerr << "Renderer failed to initialize" << std::endl;
        return false;
    }
    return true;
}

void Renderer::setupOpenGLState()
{
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Enable backface culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // Set default blend function
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set default viewport
    int width, height;
    m_target->getSize(width, height);
    setViewport(width, height);
}

void Renderer::setClearColor(glm::vec4 color)
{
    m_clearColor = color;
    glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::setViewport(int width, int height)
{
    glViewport(0, 0, width, height);

    // Update projection matrix if it's an identity matrix (default)
    if (m_projectionMatrix == glm::mat4(1.0f) && width > 0 && height > 0) {
        float aspect = static_cast<float>(width) / static_cast<float>(height);
        m_projectionMatrix = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
    }
}

void Renderer::beginFrame()
{
    if (!m_initialized) return;

    // Reset statistics
    resetStats();

    // Clear buffers
    glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Apply render settings
    if (m_depthTesting) {
        glEnable(GL_DEPTH_TEST);
    }
    else {
        glDisable(GL_DEPTH_TEST);
    }

    if (m_backfaceCulling) {
        glEnable(GL_CULL_FACE);
    }
    else {
        glDisable(GL_CULL_FACE);
    }

    if (m_wireframeMode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void Renderer::endFrame()
{
    if (!m_target || !m_initialized) return;

    // Swap buffers
    m_target->swapBuffers();

    // Optional: Print stats in debug mode
#ifdef _DEBUG
    if (m_stats.drawCalls > 0) {
        // std::cout << "Draw calls: " << m_stats.drawCalls 
        //           << ", Triangles: " << m_stats.trianglesDrawn << std::endl;
    }
#endif
}

void Renderer::setClearColor(glm::vec3 color)
{
    m_clearColor = glm::vec4(color, 1);
}

void Renderer::render()
{
    // Legacy render function - just clears the screen
    beginFrame();
    endFrame();
}

void Renderer::renderModel(Model& model, const glm::mat4& transform)
{
    if (!model.isValid()) return;

    // Use default shader
    m_defaultShader.Use();

    // Set matrices
    m_defaultShader.SetMat4("model", transform);
    m_defaultShader.SetMat4("view", m_viewMatrix);
    m_defaultShader.SetMat4("projection", m_projectionMatrix);

    // Set default lighting
    m_defaultShader.SetVec3("lightPos", glm::vec3(5.0f, 5.0f, 5.0f));
    m_defaultShader.SetVec3("viewPos", glm::vec3(0.0f, 0.0f, 3.0f));
    m_defaultShader.SetVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    m_defaultShader.SetVec3("objectColor", glm::vec3(0.8f, 0.8f, 0.8f));
    m_defaultShader.SetFloat("ambientStrength", 0.1f);
    m_defaultShader.SetFloat("specularStrength", 0.5f);

    // Draw all meshes
    for (const auto& mesh : model.getMeshes()) {
        if (mesh) {
            mesh->draw(m_defaultShader);
            m_stats.drawCalls++;
            m_stats.trianglesDrawn += mesh->getIndices().size() / 3;
            m_stats.verticesDrawn += mesh->getVertices().size();
        }
    }
}

void Renderer::renderMesh(Mesh& mesh, const glm::mat4& transform)
{
    if (mesh.isEmpty()) return;

    m_defaultShader.Use();

    m_defaultShader.SetMat4("model", transform);
    m_defaultShader.SetMat4("view", m_viewMatrix);
    m_defaultShader.SetMat4("projection", m_projectionMatrix);

    m_defaultShader.SetVec3("lightPos", glm::vec3(5.0f, 5.0f, 5.0f));
    m_defaultShader.SetVec3("viewPos", glm::vec3(0.0f, 0.0f, 3.0f));
    m_defaultShader.SetVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    m_defaultShader.SetVec3("objectColor", glm::vec3(0.8f, 0.8f, 0.8f));
    m_defaultShader.SetFloat("ambientStrength", 0.1f);
    m_defaultShader.SetFloat("specularStrength", 0.5f);

    mesh.draw(m_defaultShader);
    m_stats.drawCalls++;
    m_stats.trianglesDrawn += mesh.getIndices().size() / 3;
    m_stats.verticesDrawn += mesh.getVertices().size();
}

//void Renderer::renderScene(const Scene& scene)
//{
//    // This would iterate through all objects in the scene
//    // For now, we'll leave it empty
//    // In a real implementation, you would:
//    // 1. Sort objects (transparent vs opaque)
//    // 2. Apply frustum culling
//    // 3. Render each object with its transform
//    // 4. Handle lights, shadows, etc.
//}

void Renderer::setViewMatrix(const glm::mat4& view)
{
    m_viewMatrix = view;
}

void Renderer::setProjectionMatrix(const glm::mat4& projection)
{
    m_projectionMatrix = projection;
}

void Renderer::setDepthTesting(bool enable)
{
    m_depthTesting = enable;
}

void Renderer::setBackfaceCulling(bool enable)
{
    m_backfaceCulling = enable;
}

void Renderer::setWireframeMode(bool enable)
{
    m_wireframeMode = enable;
}

void Renderer::checkGLError(const char* function)
{
    GLenum error = glGetError();
    while (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error in " << function << ": ";
        switch (error) {
        case GL_INVALID_ENUM: std::cerr << "GL_INVALID_ENUM"; break;
        case GL_INVALID_VALUE: std::cerr << "GL_INVALID_VALUE"; break;
        case GL_INVALID_OPERATION: std::cerr << "GL_INVALID_OPERATION"; break;
        case GL_OUT_OF_MEMORY: std::cerr << "GL_OUT_OF_MEMORY"; break;
        default: std::cerr << "Unknown error: " << error; break;
        }
        std::cerr << std::endl;
        error = glGetError();
    }
}