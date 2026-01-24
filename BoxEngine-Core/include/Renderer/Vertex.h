#pragma once


#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tangent;
    glm::vec3 bitangent;

    // For skeletal animation (Bethesda games use this)
    glm::ivec4 boneIDs;
    glm::vec4 boneWeights;
};

// For simple rendering for now
struct SimpleVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};