// Vertex.h
#pragma once
#include <glm/glm.hpp>

// Use #pragma pack to ensure proper alignment
#pragma pack(push, 1)
struct Vertex {
    glm::vec3 position;      // 12 bytes
    glm::vec3 normal;        // 12 bytes
    glm::vec2 texCoords;     // 8 bytes
    glm::vec3 tangent;       // 12 bytes
    glm::vec3 bitangent;     // 12 bytes
    // Total: 56 bytes

    Vertex() = default;

    Vertex(glm::vec3 pos, glm::vec3 norm = glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec2 tex = glm::vec2(0.0f))
        : position(pos), normal(norm), texCoords(tex),
        tangent(1.0f, 0.0f, 0.0f), bitangent(0.0f, 0.0f, 1.0f) {
    }
};
#pragma pack(pop)

// Also check that sizeof(Vertex) is what you expect
static_assert(sizeof(Vertex) == 56, "Vertex struct has wrong size!");