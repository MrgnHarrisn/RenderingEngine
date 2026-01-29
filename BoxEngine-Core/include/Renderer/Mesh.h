#pragma once

#include "Shader.h"
#include "Vertex.h"
#include "Texture.h"
#include <memory>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <utility> // for std::pair

class Mesh
{
public:
    Mesh() = default;
    Mesh(const std::vector<Vertex>& vertices,
        const std::vector<unsigned int>& indices,
        const std::vector<std::shared_ptr<Texture>>& textures = {});
    Mesh(const Mesh& other);

    // Add move constructor for efficiency
    Mesh(std::vector<Vertex>&& vertices,
        std::vector<unsigned int>&& indices,
        std::vector<std::shared_ptr<Texture>>&& textures = {});

    // setting vertices and indices
    void setVertices(const std::vector<Vertex>& vertices);
    void setVertices(std::vector<Vertex>&& vertices); // Move version
    void setIndices(const std::vector<unsigned int>& indices);
    void setIndices(std::vector<unsigned int>&& indices); // Move version

    // Texture management
    void addTexture(const std::shared_ptr<Texture>& texture);
    void setTextures(const std::vector<std::shared_ptr<Texture>>& textures);
    const std::vector<std::shared_ptr<Texture>>& getTextures() const { return m_textures; }
    void clearTextures() { m_textures.clear(); }

    // Material properties (for non-textured materials)
    void setMaterial(const glm::vec3& ambient, const glm::vec3& diffuse,
        const glm::vec3& specular, float shininess);
    const glm::vec3& getAmbient() const { return m_ambient; }
    const glm::vec3& getDiffuse() const { return m_diffuse; }
    const glm::vec3& getSpecular() const { return m_specular; }
    float getShininess() const { return m_shininess; }

    // data accessors - make them const!
    const std::vector<Vertex>& getVertices() const { return m_vertices; }
    const std::vector<unsigned int>& getIndices() const { return m_indices; }

    // Calculate bounds
    void calculateBounds();
    const glm::vec3& getMinBounds() const { return m_minBounds; }
    const glm::vec3& getMaxBounds() const { return m_maxBounds; }
    glm::vec3 getCenter() const { return m_center; }
    float getBoundingSphereRadius() const { return m_boundingSphereRadius; }

    // Convenience method for bounds
    std::pair<glm::vec3, glm::vec3> getBounds() const {
        return { m_minBounds, m_maxBounds };
    }

    // Check if mesh is valid
    bool isEmpty() const { return m_vertices.empty(); }

    // memory usage
    size_t getMemoryUsage() const {
        return m_vertices.size() * sizeof(Vertex) +
            m_indices.size() * sizeof(unsigned int);
    }

    void draw(const Shader& shader);

    // Transform mesh (for instancing support)
    void transform(const glm::mat4& transform);

    // operator overloading
    Mesh& operator=(const Mesh& other);
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    ~Mesh();

private:
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<std::shared_ptr<Texture>> m_textures;

    // Material properties (used when no textures are present)
    glm::vec3 m_ambient = glm::vec3(0.1f);
    glm::vec3 m_diffuse = glm::vec3(0.8f);
    glm::vec3 m_specular = glm::vec3(0.5f);
    float m_shininess = 32.0f;

    // OpenGL buffers
    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;
    unsigned int m_EBO = 0;

    // Add these private methods
    void setupBuffers();
    void updateBuffers();
    void cleanupBuffers();

    // bounds
    glm::vec3 m_minBounds;
    glm::vec3 m_maxBounds;
    glm::vec3 m_center;
    float m_boundingSphereRadius;

    // material info
    std::string m_materialName;
};