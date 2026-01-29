#pragma once

#include "Mesh.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>
#include <assimp/scene.h>
#include <unordered_map>

class Model
{
public:
    Model() = default;

    // load model from file with assimp
    bool loadFromFile(const std::string& filepath);

    // Load with custom flags
    bool loadFromFile(const std::string& filepath, unsigned int assimpFlags);

    // Model information
    const std::string& getFilePath() const { return m_filepath; }
    const std::vector<std::shared_ptr<Mesh>>& getMeshes() const { return m_meshes; }

    // open-world model stuff for LODs
    struct LODLevel {
        std::shared_ptr<Model> model;
        float switchDistance;
    };

    void addLODLevel(const std::shared_ptr<Model>& lodModel, float distance);
    const std::vector<LODLevel>& getLODLevels() const { return m_lodLevels; }

    // memory management
    size_t getTotalMemoryUsage() const;
    void clear();

    void addMesh(const std::shared_ptr<Mesh>& mesh);

    // model meta-data
    const glm::vec3& getCenter() const { return m_center; }
    float getBoundingRadius() const { return m_boundingRadius; }

    // Bounds for frustum culling
    const glm::vec3& getMinBounds() const { return m_minBounds; }
    const glm::vec3& getMaxBounds() const { return m_maxBounds; }

    // Check if model is valid
    bool isValid() const { return !m_meshes.empty(); }

    ~Model();

private:
    // Process Assimp data
    void processNode(aiNode* node, const aiScene* scene);
    std::shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);

    // Calculate model bounds
    void calculateModelBounds();

    std::vector<std::shared_ptr<Mesh>> m_meshes;

    // for culling and spatial queries
    glm::vec3 m_center;
    glm::vec3 m_minBounds;
    glm::vec3 m_maxBounds;
    float m_boundingRadius;

    std::string m_directory;
    std::string m_filepath;

    // LOD support
    std::vector<LODLevel> m_lodLevels;

    // For debugging/optimization
    size_t m_totalVertexCount = 0;
    size_t m_totalTriangleCount = 0;

    // Store loaded textures to avoid duplicates
    std::vector<std::shared_ptr<Texture>> m_loadedTextures;

    // Texture loading helper
    std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial* mat,
        aiTextureType type,
        TextureType textureType);

    std::shared_ptr<Texture> Model::createDefaultTexture(TextureType type);
};