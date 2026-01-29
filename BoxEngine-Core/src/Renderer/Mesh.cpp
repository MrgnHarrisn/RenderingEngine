#include "Renderer/Mesh.h"
#include <glad/glad.h>
#include <algorithm>
#include <limits>
#include <cmath>
#include <iostream>

Mesh::Mesh(const std::vector<Vertex>& vertices,
    const std::vector<unsigned int>& indices,
    const std::vector<std::shared_ptr<Texture>>& textures)
    : m_vertices(vertices), m_indices(indices), m_textures(textures)
{
    calculateBounds();
    setupBuffers();
}

Mesh::Mesh(std::vector<Vertex>&& vertices,
    std::vector<unsigned int>&& indices,
    std::vector<std::shared_ptr<Texture>>&& textures)
    : m_vertices(std::move(vertices)), m_indices(std::move(indices)),
    m_textures(std::move(textures))
{
    calculateBounds();
    setupBuffers();
}

Mesh::~Mesh()
{
    cleanupBuffers();
}

// Prevent copying because of OpenGL 
Mesh::Mesh(const Mesh& other)
    : m_vertices(other.m_vertices), m_indices(other.m_indices),
    m_minBounds(other.m_minBounds), m_maxBounds(other.m_maxBounds),
    m_center(other.m_center), m_boundingSphereRadius(other.m_boundingSphereRadius),
    m_materialName(other.m_materialName)
{
    setupBuffers(); // Create new OpenGL buffers ig
}

Mesh& Mesh::operator=(const Mesh& other)
{
    if (this != &other) {
        cleanupBuffers();
        m_vertices = other.m_vertices;
        m_indices = other.m_indices;
        m_minBounds = other.m_minBounds;
        m_maxBounds = other.m_maxBounds;
        m_center = other.m_center;
        m_boundingSphereRadius = other.m_boundingSphereRadius;
        m_materialName = other.m_materialName;
        setupBuffers();
    }
    return *this;
}

// Move operations for meshes
Mesh::Mesh(Mesh&& other) noexcept
    : m_vertices(std::move(other.m_vertices)),
    m_indices(std::move(other.m_indices)),
    m_minBounds(other.m_minBounds),
    m_maxBounds(other.m_maxBounds),
    m_center(other.m_center),
    m_boundingSphereRadius(other.m_boundingSphereRadius),
    m_materialName(std::move(other.m_materialName)),
    m_VAO(other.m_VAO),
    m_VBO(other.m_VBO),
    m_EBO(other.m_EBO)
{
    other.m_VAO = 0;
    other.m_VBO = 0;
    other.m_EBO = 0;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    if (this != &other) {
        cleanupBuffers();

        m_vertices = std::move(other.m_vertices);
        m_indices = std::move(other.m_indices);
        m_minBounds = other.m_minBounds;
        m_maxBounds = other.m_maxBounds;
        m_center = other.m_center;
        m_boundingSphereRadius = other.m_boundingSphereRadius;
        m_materialName = std::move(other.m_materialName);

        m_VAO = other.m_VAO;
        m_VBO = other.m_VBO;
        m_EBO = other.m_EBO;

        other.m_VAO = 0;
        other.m_VBO = 0;
        other.m_EBO = 0;
    }
    return *this;
}

void Mesh::setupBuffers()
{
    if (m_vertices.empty()) return;

    // Generate buffers
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    if (!m_indices.empty()) {
        glGenBuffers(1, &m_EBO);
    }

    // Bind VAO
    glBindVertexArray(m_VAO);

    // Bind and set vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex),
        m_vertices.data(), GL_STATIC_DRAW);

    // Bind and set element buffer if we have indices
    if (!m_indices.empty()) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int),
            m_indices.data(), GL_STATIC_DRAW);
    }

    // Set vertex attribute pointers
    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (void*)offsetof(Vertex, position));

    // Normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (void*)offsetof(Vertex, normal));

    // Texture coordinates attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (void*)offsetof(Vertex, texCoords));

    // Tangent attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (void*)offsetof(Vertex, tangent));

    // Bitangent attribute
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (void*)offsetof(Vertex, bitangent));

    // Unbind VAO
    glBindVertexArray(0);
}

void Mesh::updateBuffers()
{
    if (m_VAO == 0) {
        setupBuffers();
        return;
    }

    // Update vertex buffer data
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex),
        m_vertices.data(), GL_STATIC_DRAW);

    // Update element buffer if we have indices
    if (!m_indices.empty() && m_EBO != 0) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int),
            m_indices.data(), GL_STATIC_DRAW);
    }
}

void Mesh::cleanupBuffers()
{
    if (m_VAO != 0) {
        glDeleteVertexArrays(1, &m_VAO);
        m_VAO = 0;
    }
    if (m_VBO != 0) {
        glDeleteBuffers(1, &m_VBO);
        m_VBO = 0;
    }
    if (m_EBO != 0) {
        glDeleteBuffers(1, &m_EBO);
        m_EBO = 0;
    }
}

void Mesh::setVertices(const std::vector<Vertex>& vertices)
{
    m_vertices = vertices;
    calculateBounds();
    updateBuffers();
}

void Mesh::setVertices(std::vector<Vertex>&& vertices)
{
    m_vertices = std::move(vertices);
    calculateBounds();
    updateBuffers();
}

void Mesh::setIndices(const std::vector<unsigned int>& indices)
{
    m_indices = indices;
    if (m_EBO == 0 && !m_indices.empty()) {
        // Need to create EBO if it doesn't exist
        glGenBuffers(1, &m_EBO);
    }
    updateBuffers();
}

void Mesh::setIndices(std::vector<unsigned int>&& indices)
{
    m_indices = std::move(indices);
    if (m_EBO == 0 && !m_indices.empty()) {
        glGenBuffers(1, &m_EBO);
    }
    updateBuffers();
}

void Mesh::addTexture(const std::shared_ptr<Texture>& texture)
{
    if (texture && texture->isValid())
    {
        m_textures.push_back(texture);
    }
}

void Mesh::setTextures(const std::vector<std::shared_ptr<Texture>>& textures)
{
    m_textures = textures;
}

//void Mesh::setMaterial(const glm::vec3& ambient, const glm::vec3& diffuse,
//    const glm::vec3& specular, float shininess)
//{
//    m_ambient = ambient;
//    m_diffuse = diffuse;
//    m_specular = specular;
//    m_shininess = shininess;
//}

void Mesh::calculateBounds()
{
    if (m_vertices.empty()) {
        m_minBounds = glm::vec3(0.0f);
        m_maxBounds = glm::vec3(0.0f);
        m_center = glm::vec3(0.0f);
        m_boundingSphereRadius = 0.0f;
        return;
    }

    // Initialize with extreme values
    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float minZ = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float maxY = std::numeric_limits<float>::lowest();
    float maxZ = std::numeric_limits<float>::lowest();

    // Find min/max bounds
    for (const auto& vertex : m_vertices) {
        minX = std::min(minX, vertex.position.x);
        minY = std::min(minY, vertex.position.y);
        minZ = std::min(minZ, vertex.position.z);
        maxX = std::max(maxX, vertex.position.x);
        maxY = std::max(maxY, vertex.position.y);
        maxZ = std::max(maxZ, vertex.position.z);
    }

    m_minBounds = glm::vec3(minX, minY, minZ);
    m_maxBounds = glm::vec3(maxX, maxY, maxZ);
    m_center = (m_minBounds + m_maxBounds) * 0.5f;

    // Calculate bounding sphere radius
    float maxDistSq = 0.0f;
    for (const auto& vertex : m_vertices) {
        glm::vec3 diff = vertex.position - m_center;
        float distSq = glm::dot(diff, diff);
        if (distSq > maxDistSq) {
            maxDistSq = distSq;
        }
    }
    m_boundingSphereRadius = std::sqrt(maxDistSq);
}

void Mesh::setMaterial(const glm::vec3& ambient, const glm::vec3& diffuse,
    const glm::vec3& specular, float shininess)
{
    m_ambient = ambient;
    m_diffuse = diffuse;
    m_specular = specular;
    m_shininess = shininess;
}

void Mesh::draw(const Shader& shader)
{
    if (m_vertices.empty() || m_VAO == 0) return;

    // Bind textures if available
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;

    for (unsigned int i = 0; i < m_textures.size(); i++) {
        // Activate proper texture unit before binding
        glActiveTexture(GL_TEXTURE0 + i);

        // Retrieve texture info
        const auto& texture = m_textures[i];
        std::string uniformName;
        std::string number;

        switch (texture->getType()) {
        case DIFFUSE:
            number = std::to_string(diffuseNr++);
            uniformName = "material.texture_diffuse" + number;
            break;
        case SPECULAR:
            number = std::to_string(specularNr++);
            uniformName = "material.texture_specular" + number;
            break;
        case NORMAL:
            number = std::to_string(normalNr++);
            uniformName = "material.texture_normal" + number;
            break;
        case HEIGHT:
            number = std::to_string(heightNr++);
            uniformName = "material.texture_height" + number;
            break;
        case AMBIENT:
            // Ambient textures are usually treated as diffuse
            number = std::to_string(diffuseNr++);
            uniformName = "material.texture_diffuse" + number;
            break;
        }

        // Only set the uniform if it exists in the shader
        int uniformLocation = glGetUniformLocation(shader.GetID(), uniformName.c_str());
        if (uniformLocation != -1) {
            shader.SetInt(uniformName, i);
            texture->bind(i);
        }
        else {
            // If the specific uniform doesn't exist, try to use texture_diffuse1
            if (texture->getType() == DIFFUSE) {
                int fallbackLocation = glGetUniformLocation(shader.GetID(), "material.texture_diffuse1");
                if (fallbackLocation != -1) {
                    shader.SetInt("material.texture_diffuse1", i);
                    texture->bind(i);
                }
            }
        }
    }

    // Set material properties
    int useTexturesLoc = glGetUniformLocation(shader.GetID(), "material.useTextures");
    if (useTexturesLoc != -1) {
        shader.SetBool("material.useTextures", !m_textures.empty());
    }

    // Only set material colors if the shader has these uniforms
    int ambientLoc = glGetUniformLocation(shader.GetID(), "material.ambient");
    if (ambientLoc != -1) {
        shader.SetVec3("material.ambient", m_ambient);
    }

    int diffuseLoc = glGetUniformLocation(shader.GetID(), "material.diffuse");
    if (diffuseLoc != -1) {
        shader.SetVec3("material.diffuse", m_diffuse);
    }

    int specularLoc = glGetUniformLocation(shader.GetID(), "material.specular");
    if (specularLoc != -1) {
        shader.SetVec3("material.specular", m_specular);
    }

    int shininessLoc = glGetUniformLocation(shader.GetID(), "material.shininess");
    if (shininessLoc != -1) {
        shader.SetFloat("material.shininess", m_shininess);
    }

    // Bind VAO
    glBindVertexArray(m_VAO);

    // Draw based on whether we have indices or not
    if (!m_indices.empty()) {
        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    }
    else {
        // Draw vertices as triangles (assuming they're triangle lists)
        glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
    }

    // Unbind VAO
    glBindVertexArray(0);

    // Unbind textures
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::transform(const glm::mat4& transform)
{
    glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(transform)));

    for (auto& vertex : m_vertices) {
        // Transform position
        glm::vec4 transformedPos = transform * glm::vec4(vertex.position, 1.0f);
        vertex.position = glm::vec3(transformedPos);

        // Transform normal
        vertex.normal = glm::normalize(normalMatrix * vertex.normal);

        // Transform tangent and bitangent (for normal mapping)
        vertex.tangent = glm::normalize(normalMatrix * vertex.tangent);
        vertex.bitangent = glm::normalize(normalMatrix * vertex.bitangent);
    }
    calculateBounds();
    updateBuffers(); // Update GPU buffers with new vertex data
}