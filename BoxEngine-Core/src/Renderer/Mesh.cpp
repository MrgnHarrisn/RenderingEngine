#include "Renderer/Mesh.h"
#include <algorithm>
#include <limits>
#include <cmath>

Mesh::Mesh(const std::vector<Vertex>& vertices,
    const std::vector<unsigned int>& indices)
    : m_vertices(vertices), m_indices(indices)
{
    calculateBounds();
}

Mesh::Mesh(std::vector<Vertex>&& vertices,
    std::vector<unsigned int>&& indices)
    : m_vertices(std::move(vertices)), m_indices(std::move(indices))
{
    calculateBounds();
}

void Mesh::setVertices(const std::vector<Vertex>& vertices)
{
    m_vertices = vertices;
    calculateBounds();
}

void Mesh::setVertices(std::vector<Vertex>&& vertices)
{
    m_vertices = std::move(vertices);
    calculateBounds();
}

void Mesh::setIndices(const std::vector<unsigned int>& indices)
{
    m_indices = indices;
}

void Mesh::setIndices(std::vector<unsigned int>&& indices)
{
    m_indices = std::move(indices);
}

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

void Mesh::transform(const glm::mat4& transform)
{
    for (auto& vertex : m_vertices) {
        // Transform position
        glm::vec4 transformedPos = transform * glm::vec4(vertex.position, 1.0f);
        vertex.position = glm::vec3(transformedPos);

        // Transform normal (assuming uniform scaling for now)
        glm::vec4 transformedNormal = transform * glm::vec4(vertex.normal, 0.0f);
        vertex.normal = glm::normalize(glm::vec3(transformedNormal));

        // Note: For non-uniform scaling, we'd need to use the inverse transpose matrix
    }
    calculateBounds(); // Recalculate bounds after transformation
}