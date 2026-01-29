#pragma once
#include <string>


enum TextureType {
    DIFFUSE = 0,
    SPECULAR,
    NORMAL,
    HEIGHT,
    AMBIENT
};

class Texture {
public:
    Texture();
    ~Texture();
    
    // Load texture from file
    bool loadFromFile(const std::string& filepath, TextureType type);
    
    // Load texture from memory (for procedural textures)
    bool loadFromData(unsigned char* data, int width, int height, TextureType type);
    
    // Create empty texture
    bool create(int width, int height, TextureType type);
    
    // Bind texture to a texture unit
    void bind(unsigned int unit = 0) const;
    
    // Unbind texture
    void unbind() const;
    
    // Get texture ID
    unsigned int getID() const { return m_id; }
    
    // Get texture type
    TextureType getType() const { return m_type; }
    
    // Get texture path (if loaded from file)
    std::string& getPath() { return m_path; }
    
    // Get texture dimensions
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    
    // Check if texture is valid
    bool isValid() const { return m_id != 0; }
    
private:
    unsigned int m_id = 0;
    TextureType m_type;
    std::string m_path;
    int m_width = 0;
    int m_height = 0;
    int m_channels = 0;
    
    // Generate OpenGL texture
    void generateTexture();
    
    // Set texture parameters
    void setTextureParameters();

    bool Texture::fileExists(const std::string& filepath);
};