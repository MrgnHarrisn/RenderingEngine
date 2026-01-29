#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
    sampler2D texture_normal1;
    sampler2D texture_normal2;
    sampler2D texture_height1;
    sampler2D texture_height2;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    bool useTextures;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

uniform Material material;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform float ambientStrength;
uniform float specularStrength;

void main()
{
    vec3 result;
    
    if (material.useTextures) {
        // Use textures
        vec4 texColor = texture(material.texture_diffuse1, TexCoords);
        
        // If we have multiple diffuse textures, blend them
        if (textureSize(material.texture_diffuse2, 0).x > 1) {
            vec4 texColor2 = texture(material.texture_diffuse2, TexCoords);
            texColor = mix(texColor, texColor2, 0.5);
        }
        
        // Ambient lighting
        vec3 ambient = ambientStrength * lightColor;
        
        // Diffuse lighting
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;
        
        // Specular lighting
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
        vec3 specular = specularStrength * spec * lightColor;
        
        // Combine
        vec3 lighting = (ambient + diffuse + specular);
        result = lighting * texColor.rgb;
    } else {
        // Use material colors
        // Ambient lighting
        vec3 ambient = ambientStrength * lightColor * material.ambient;
        
        // Diffuse lighting
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor * material.diffuse;
        
        // Specular lighting
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = specularStrength * spec * lightColor * material.specular;
        
        // Combine
        result = (ambient + diffuse + specular) * objectColor;
    }
    
    FragColor = vec4(result, 1.0);
}