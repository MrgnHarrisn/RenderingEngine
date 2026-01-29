#include "Renderer/Texture.h"
#include <iostream>
#include <glad/glad.h>
#include <fstream>

// Define STB_IMAGE_IMPLEMENTATION before including stb_image.h
#include "stb_image.h"

Texture::Texture()
    : m_id(0)
    , m_type(DIFFUSE)
    , m_width(0)
    , m_height(0)
{
}

Texture::~Texture()
{
    if (m_id != 0) {
        glDeleteTextures(1, &m_id);
    }
}

bool Texture::loadFromFile(const std::string& filepath, TextureType type)
{
    // Make sure file actually exists
    if (!fileExists(filepath)) {
        std::cerr << "ERROR: Texture file does not exist: " << filepath << std::endl;
        return false;
    }

    // Clean up any existing texture
    if (m_id != 0) {
        glDeleteTextures(1, &m_id);
        m_id = 0;
    }

    // Reset dimensions and number color_channels
    m_width = 0;
    m_height = 0;
    m_channels = 0;

    // Set flip option based on texture type
    stbi_set_flip_vertically_on_load(type == DIFFUSE); // Only flip diffuse textures typically

    // Load image data
    unsigned char* data = stbi_load(filepath.c_str(), &m_width, &m_height, &m_channels, 0);

    if (!data) {
        std::cerr << "ERROR: Failed to load texture: " << filepath << std::endl;
        std::cerr << "STB Reason: " << stbi_failure_reason() << std::endl;
        return false;
    }

    m_type = type;
    m_path = filepath;

    // Generate and bind texture
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Determine format based on number of channels
    GLenum format = GL_RGB;
    GLenum internalFormat = GL_RGB;

    if (m_channels == 1) {
        format = GL_RED;
        internalFormat = GL_RED;
    }
    else if (m_channels == 3) {
        format = GL_RGB;
        internalFormat = GL_RGB;
    }
    else if (m_channels == 4) {
        format = GL_RGBA;
        internalFormat = GL_RGBA;
    }

    // Upload texture data
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Free image data
    stbi_image_free(data);

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
#ifndef NDEBUG
    std::cout << "Loaded texture: " << filepath
        << " (" << m_width << "x" << m_height
        << ", channels: " << m_channels << ")" << std::endl;
#endif
    return true;
}

bool Texture::loadFromData(unsigned char* data, int width, int height, TextureType type)
{
    if (!data || width <= 0 || height <= 0) {
        return false;
    }

    // Clean up any existing texture
    if (m_id != 0) {
        glDeleteTextures(1, &m_id);
        m_id = 0;
    }

    m_width = width;
    m_height = height;
    m_type = type;
    m_path = "procedural";

    // Generate and bind texture
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload texture data (assuming RGB)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

bool Texture::create(int width, int height, TextureType type)
{
    if (width <= 0 || height <= 0) {
        return false;
    }

    // Clean up any existing texture
    if (m_id != 0) {
        glDeleteTextures(1, &m_id);
        m_id = 0;
    }

    m_width = width;
    m_height = height;
    m_type = type;
    m_path = "created";

    // Generate and bind texture
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Create empty texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void Texture::bind(unsigned int unit) const
{
    if (m_id == 0) return;

    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

bool Texture::fileExists(const std::string& filepath)
{
    std::ifstream file(filepath);
    return file.good();
}
