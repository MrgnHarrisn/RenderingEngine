#include "Core/Window.h"
#include "Renderer/Renderer.h"
#include "Renderer/Model.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


float dist = 1;

// this main is completely temporary, it WILL NOT stay like this
int main() {
    // Create window
    Window window(800, 600, "3D Renderer");
    if (!window.isValid()) {
        std::cerr << "Failed to create window" << std::endl;
        return -1;
    }

    // Create renderer attached to window
    Renderer renderer(&window);
    if (!renderer.init()) {
        std::cerr << "Failed to initialize renderer" << std::endl;
        return -1;
    }

    // Set clear color
    renderer.setClearColor(glm::vec3(0.1f, 0.1f, 0.15f));

    // Load a 3D model
    Model model;
    bool modelLoaded = model.loadFromFile("assets/Models/head/adamHead.gltf");

    // temporary
    if (!modelLoaded) {
        std::cout << "Creating fallback cube..." << std::endl;
        // Create a simple cube as fallback
        std::vector<Vertex> vertices = {
            // Front face
            Vertex{glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
            Vertex{glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
            Vertex{glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
            Vertex{glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},

            // Back face
            Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)},
            Vertex{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
            Vertex{glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)},
            Vertex{glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)}
        };

        std::vector<unsigned int> indices = {
            // Front
            0, 1, 2, 2, 3, 0,
            // Back
            4, 5, 6, 6, 7, 4,
            // Top
            3, 2, 6, 6, 7, 3,
            // Bottom
            0, 1, 5, 5, 4, 0,
            // Right
            1, 5, 6, 6, 2, 1,
            // Left
            0, 4, 7, 7, 3, 0
        };

        std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(vertices, indices);
        model.addMesh(mesh);
    }

    // Set up camera matrices this should be done in a camera class
    glm::mat4 view = glm::lookAt(
        glm::vec3(dist, 2.0f, dist),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    int width, height;
    window.getSize(width, height);
    float aspect = static_cast<float>(width) / static_cast<float>(height);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

    renderer.setViewMatrix(view);
    renderer.setProjectionMatrix(projection);

    // Main loop
    float rotation = 0.0f;
    while (!window.shouldClose()) {
        // Update rotation
        rotation += 0.01f;

        // Create model transform
        glm::mat4 modelTransform = glm::mat4(1.0f);
        modelTransform = glm::rotate(modelTransform, rotation, glm::vec3(0.0f, 1.0f, 0.0f));

        // Update camera orbit
        float camX = sin(rotation) * dist;
        float camZ = cos(rotation) * dist;
        view = glm::lookAt(
            glm::vec3(dist, 1.0f, dist),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );
        renderer.setViewMatrix(view);

        // Begin frame
        renderer.beginFrame();

        // Render the model
        renderer.renderModel(model, modelTransform);

        // End frame
        renderer.endFrame();

        // Process window events
        window.pollEvents();
    }
    return 0;
}