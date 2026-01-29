#pragma once
#include <string>

enum TextureType {
    DIFFUSE = 0,
    SPECULAR,
    NORMAL,
    HEIGHT,
    AMBIENT
};

struct Texture {
    unsigned int id;
    TextureType type;
    std::string path;

    Texture() : id(0) {}
    Texture(unsigned int id, TextureType type, const std::string& path)
        : id(id), type(type), path(path) {
    }
};