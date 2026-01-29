#pragma once
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
    Shader();
    ~Shader();

    // Create shader from source files
    bool LoadFromFile(const std::string& vertexPath, const std::string& fragmentPath);

    // Create shader from source code
    bool LoadFromSource(const std::string& vertexSource, const std::string& fragmentSource);

    // Use the shader
    void Use() const;

    // Utility uniform functions
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetVec2(const std::string& name, const glm::vec2& value) const;
    void SetVec3(const std::string& name, const glm::vec3& value) const;
    void SetVec4(const std::string& name, const glm::vec4& value) const;
    void SetMat2(const std::string& name, const glm::mat2& mat) const;
    void SetMat3(const std::string& name, const glm::mat3& mat) const;
    void SetMat4(const std::string& name, const glm::mat4& mat) const;

    // Get shader ID
    unsigned int GetID() const { return m_ID; }

private:
    unsigned int m_ID;
    mutable std::unordered_map<std::string, int> m_UniformLocationCache;

    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    int GetUniformLocation(const std::string& name) const;
    std::string ReadFile(const std::string& filepath);
};