#include "Renderer/Model.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <iostream>
#include <algorithm>
#include <filesystem>

// Default Assimp flags for game assets
static const unsigned int DEFAULT_ASSIMP_FLAGS =
        aiProcess_Triangulate |
        aiProcess_GenNormals |
        aiProcess_CalcTangentSpace |
        aiProcess_JoinIdenticalVertices |
        aiProcess_ImproveCacheLocality |
        aiProcess_SortByPType |
        aiProcess_OptimizeMeshes |
        aiProcess_ValidateDataStructure;

bool Model::loadFromFile(const std::string& filepath)
{
    return loadFromFile(filepath, DEFAULT_ASSIMP_FLAGS);
}

bool Model::loadFromFile(const std::string& filepath, unsigned int assimpFlags)
{
    Assimp::Importer importer;
    m_filepath = filepath;

    const aiScene* scene = importer.ReadFile(filepath, assimpFlags);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Assimp error: " << importer.GetErrorString() << std::endl;
        return false;
    }

    m_directory = filepath.substr(0, filepath.find_last_of("/\\"));
    m_meshes.clear();
    m_totalVertexCount = 0;
    m_totalTriangleCount = 0;

    // Process all meshes
    processNode(scene->mRootNode, scene);

    if (m_meshes.empty()) {
        std::cerr << "Warning: No meshes found in " << filepath << std::endl;
        return false;
    }

    // Calculate overall model bounds
    calculateModelBounds();
#ifndef NDEBUG
    std::cout << "Loaded model: " << filepath
        << "\n  Meshes: " << m_meshes.size()
        << "\n  Vertices: " << m_totalVertexCount
        << "\n  Triangles: " << m_totalTriangleCount
        << "\n  Memory: " << getTotalMemoryUsage() / 1024 << " KB"
        << "\n  Bounds: [" << m_minBounds.x << ", " << m_minBounds.y << ", " << m_minBounds.z
        << "] to [" << m_maxBounds.x << ", " << m_maxBounds.y << ", " << m_maxBounds.z << "]"
        << "\n  Bounding sphere radius: " << m_boundingRadius
        << std::endl;
#endif
    return true;
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // Process all the node's meshes
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        auto processedMesh = processMesh(mesh, scene);
        if (processedMesh) {
            m_meshes.push_back(processedMesh);
            m_totalVertexCount += processedMesh->getVertices().size();
            m_totalTriangleCount += processedMesh->getIndices().size() / 3;
        }
    }

    // Process children
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

std::shared_ptr<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::shared_ptr<Texture>> textures;

    // Pre-allocate memory for efficiency
    vertices.reserve(mesh->mNumVertices);
    indices.reserve(mesh->mNumFaces * 3); // 3 for triangles

    // Process vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        // Position
        vertex.position = glm::vec3(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        );

        // Normals
        if (mesh->HasNormals()) {
            vertex.normal = glm::vec3(
                mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z
            );
        }
        else {
            vertex.normal = glm::vec3(0.0f, 1.0f, 0.0f); // Default up
        }

        // Texture coordinates
        if (mesh->mTextureCoords[0]) {
            vertex.texCoords = glm::vec2(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            );
        }
        else {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }

        // Tangents and bitangents
        if (mesh->HasTangentsAndBitangents()) {
            vertex.tangent = glm::vec3(
                mesh->mTangents[i].x,
                mesh->mTangents[i].y,
                mesh->mTangents[i].z
            );
            vertex.bitangent = glm::vec3(
                mesh->mBitangents[i].x,
                mesh->mBitangents[i].y,
                mesh->mBitangents[i].z
            );
        }
        else {
            vertex.tangent = glm::vec3(1.0f, 0.0f, 0.0f);
            vertex.bitangent = glm::vec3(0.0f, 0.0f, 1.0f);
        }

        vertices.push_back(vertex);
    }

    // Process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // Process material/textures
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        // Load diffuse textures
        std::vector<std::shared_ptr<Texture>> diffuseMaps = loadMaterialTextures(
            material, aiTextureType_DIFFUSE, DIFFUSE);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        // Load specular textures
        std::vector<std::shared_ptr<Texture>> specularMaps = loadMaterialTextures(
            material, aiTextureType_SPECULAR, SPECULAR);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        // Load normal maps
        std::vector<std::shared_ptr<Texture>> normalMaps = loadMaterialTextures(
            material, aiTextureType_NORMALS, NORMAL);
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

        // Load height maps
        std::vector<std::shared_ptr<Texture>> heightMaps = loadMaterialTextures(
            material, aiTextureType_HEIGHT, HEIGHT);
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    }

    // Create and return mesh
    auto result = std::make_shared<Mesh>(std::move(vertices), std::move(indices), std::move(textures));
    return result;
}

void Model::calculateModelBounds()
{
    if (m_meshes.empty()) {
        m_minBounds = glm::vec3(0.0f);
        m_maxBounds = glm::vec3(0.0f);
        m_center = glm::vec3(0.0f);
        m_boundingRadius = 0.0f;
        return;
    }

    // Initialize with first mesh bounds
    m_minBounds = m_meshes[0]->getMinBounds();
    m_maxBounds = m_meshes[0]->getMaxBounds();

    // Expand to include all meshes
    for (const auto& mesh : m_meshes) {
        const glm::vec3& meshMin = mesh->getMinBounds();
        const glm::vec3& meshMax = mesh->getMaxBounds();

        m_minBounds.x = std::min(m_minBounds.x, meshMin.x);
        m_minBounds.y = std::min(m_minBounds.y, meshMin.y);
        m_minBounds.z = std::min(m_minBounds.z, meshMin.z);

        m_maxBounds.x = std::max(m_maxBounds.x, meshMax.x);
        m_maxBounds.y = std::max(m_maxBounds.y, meshMax.y);
        m_maxBounds.z = std::max(m_maxBounds.z, meshMax.z);
    }

    m_center = (m_minBounds + m_maxBounds) * 0.5f;

    // Calculate bounding sphere radius
    float maxDistSq = 0.0f;
    for (const auto& mesh : m_meshes) {
        glm::vec3 diffMin = mesh->getMinBounds() - m_center;
        glm::vec3 diffMax = mesh->getMaxBounds() - m_center;

        maxDistSq = std::max(maxDistSq, glm::dot(diffMin, diffMin));
        maxDistSq = std::max(maxDistSq, glm::dot(diffMax, diffMax));
    }

    m_boundingRadius = std::sqrt(maxDistSq);
}

std::vector<std::shared_ptr<Texture>> Model::loadMaterialTextures(
    aiMaterial* mat,
    aiTextureType aiType,
    TextureType textureType)
{
    std::vector<std::shared_ptr<Texture>> textures;

    unsigned int textureCount = mat->GetTextureCount(aiType);

#ifndef NDEBUG
    std::cout << "Loading " << textureCount << " textures of type "
        << aiType << " (our type: " << textureType << ")" << std::endl;
#endif
    for (unsigned int i = 0; i < textureCount; i++) {
        aiString aiPath;

        // Get the texture path from Assimp
        if (mat->GetTexture(aiType, i, &aiPath) != AI_SUCCESS) {
            std::cerr << "Failed to get texture path for texture " << i << std::endl;
            continue;
        }

        std::string texturePath = aiPath.C_Str();
#ifndef NDEBUG
        std::cout << "Found texture: " << texturePath << std::endl;
#endif
        // Try different path strategies
        std::string fullPath;

        // Strategy 1: Check if it's an absolute path
        if (texturePath.find(':') != std::string::npos ||
            texturePath[0] == '/' || texturePath[0] == '\\') {
            fullPath = texturePath;
        }
        // Strategy 2: Relative to model directory
        else {
            fullPath = m_directory + "/" + texturePath;

            // Try with just the filename if the above doesn't work
            if (!std::filesystem::exists(fullPath)) {
                std::filesystem::path p(texturePath);
                fullPath = m_directory + "/" + p.filename().string();
            }
        }
#ifndef NDEBUG
        std::cout << "Trying to load texture from: " << fullPath << std::endl;
#endif
        // Check if texture was already loaded
        bool skip = false;
        for (const auto& tex : m_loadedTextures) {
            if (tex->getPath() == fullPath) {
#ifndef NDEBUG
                std::cout << "Texture already loaded, reusing: " << fullPath << std::endl;
#endif
                textures.push_back(tex);
                skip = true;
                break;
            }
        }

        if (!skip) {
            try {
                // Load new texture
                auto texture = std::make_shared<Texture>();

                // Special handling for different texture types
                if (texture->loadFromFile(fullPath, textureType)) {
#ifndef NDEBUG
                    std::cout << "Successfully loaded texture: " << fullPath << std::endl;
#endif
                    textures.push_back(texture);
                    m_loadedTextures.push_back(texture);
                }
                else {
                    std::cerr << "Failed to load texture: " << fullPath << std::endl;

                    // Try to load a fallback texture
                    std::string fallbackPath;

                    // Different fallbacks based on texture type
                    switch (textureType) {
                    case DIFFUSE:
                        fallbackPath = "textures/default_diffuse.jpg";
                        break;
                    case SPECULAR:
                        fallbackPath = "textures/default_specular.jpg";
                        break;
                    case NORMAL:
                        fallbackPath = "textures/default_normal.jpg";
                        break;
                    default:
                        fallbackPath = "textures/default.jpg";
                    }

                    if (texture->loadFromFile(fallbackPath, textureType)) {
#ifndef NDEBUG
                        std::cout << "Using fallback texture: " << fallbackPath << std::endl;
#endif
                        textures.push_back(texture);
                        m_loadedTextures.push_back(texture);
                    }
                }
            }
            catch (const std::exception& e) {
                std::cerr << "Exception loading texture: " << e.what() << std::endl;
            }
        }
    }

    // If no textures were found for this type, create a default one
    if (textures.empty()) {
        std::cout << "No textures of type " << textureType;
#ifndef NDEBUG
        std::cout << " found, creating default.";
#endif
        std::cout << std::endl;
        // Create a default procedural texture
        auto defaultTexture = createDefaultTexture(textureType);
        if (defaultTexture) {
            textures.push_back(defaultTexture);
            m_loadedTextures.push_back(defaultTexture);
        }
    }

    return textures;
}

// Helper function to create default procedural textures
std::shared_ptr<Texture> Model::createDefaultTexture(TextureType type)
{
    auto texture = std::make_shared<Texture>();

    // Create a small procedural texture (4x4 pixels)
    const int width = 4;
    const int height = 4;
    unsigned char data[width * height * 3]; // RGB

    glm::vec3 color;

    // Choose color based on texture type
    switch (type) {
    case DIFFUSE:
        color = glm::vec3(0.8f, 0.8f, 0.8f); // Gray
        break;
    case SPECULAR:
        color = glm::vec3(0.5f, 0.5f, 0.5f); // Dark gray
        break;
    case NORMAL:
        color = glm::vec3(0.5f, 0.5f, 1.0f); // Blueish (normal map default)
        break;
    default:
        color = glm::vec3(1.0f, 0.0f, 1.0f); // Magenta (error color)
    }

    // Fill the texture data
    for (int i = 0; i < width * height * 3; i += 3) {
        data[i] = static_cast<unsigned char>(color.r * 255);
        data[i + 1] = static_cast<unsigned char>(color.g * 255);
        data[i + 2] = static_cast<unsigned char>(color.b * 255);
    }

    if (texture->loadFromData(data, width, height, type)) {
#ifndef NDEBUG
        std::cout << "Created default procedural texture for type: " << type << std::endl;
#endif
        return texture;
    }

    return nullptr;
}

size_t Model::getTotalMemoryUsage() const
{
    size_t total = 0;
    for (const auto& mesh : m_meshes) {
        total += mesh->getMemoryUsage();
    }
    return total;
}

void Model::clear()
{
    m_meshes.clear();
    m_lodLevels.clear();
    m_totalVertexCount = 0;
    m_totalTriangleCount = 0;
}

void Model::addMesh(const std::shared_ptr<Mesh>& mesh)
{
    if (mesh) {
        m_meshes.push_back(mesh);
        // Update statistics
        m_totalVertexCount += mesh->getVertices().size();
        m_totalTriangleCount += mesh->getIndices().size() / 3;

        // Recalculate model bounds to include new mesh
        calculateModelBounds();
    }
}

void Model::addLODLevel(const std::shared_ptr<Model>& lodModel, float distance)
{
    if (lodModel && lodModel->isValid()) {
        m_lodLevels.push_back({ lodModel, distance });
        // Sort by distance (closest first)
        std::sort(m_lodLevels.begin(), m_lodLevels.end(),
            [](const LODLevel& a, const LODLevel& b) {
                return a.switchDistance < b.switchDistance;
            });
    }
}

Model::~Model()
{
    m_lodLevels.clear();
    m_meshes.clear();
}