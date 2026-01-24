#pragma once

#include "Vertex.h"
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
        const std::vector<unsigned int>& indices);

    // Add move constructor for efficiency
    Mesh(std::vector<Vertex>&& vertices,
        std::vector<unsigned int>&& indices);

    // setting vertices and indices
    void setVertices(const std::vector<Vertex>& vertices);
    void setVertices(std::vector<Vertex>&& vertices); // Move version
    void setIndices(const std::vector<unsigned int>& indices);
    void setIndices(std::vector<unsigned int>&& indices); // Move version

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

    // Transform mesh (for instancing support)
    void transform(const glm::mat4& transform);

private:
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;

    // bounds
    glm::vec3 m_minBounds;
    glm::vec3 m_maxBounds;
    glm::vec3 m_center;
    float m_boundingSphereRadius;

    // material info
    std::string m_materialName;
};