#pragma once

#include <string>
#include <memory>
#include <iostream>

#include "Renderer/Model.h"
#include "Core/Window.h"
#include "Renderer/Renderer.h"
#include "World/World.h"

class Engine
{
public:
	/// <summary>
	/// Empty constructor for some reason
	/// </summary>
	Engine();

	/// <summary>
	/// Basic constructor
	/// </summary>
	/// <param name="width">Width of window</param>
	/// <param name="height">Height of window</param>
	/// <param name="title">Title of window</param>
	Engine(unsigned int width, unsigned int height, const std::string& title);

	/// <summary>
	/// Set the clear color of the screen
	/// </summary>
	/// <param name="r">red</param>
	/// <param name="g">green</param>
	/// <param name="b">blue</param>
	void setClearColor(float r, float g, float b);

	/// <summary>
	/// Run the engine
	/// </summary>
	void run();

	/// <summary>
	/// Initialize Engine
	/// </summary>
	void init();

	/// <summary>
	/// Destructor for the window
	/// </summary>
	~Engine();
private:
	Window*		m_window	= nullptr;
	Renderer*	m_renderer	= nullptr;
	World*		m_world		= nullptr;

	std::unordered_map<std::string, std::shared_ptr<Model>> m_loadedModels;

	// Test function
	void testModelLoading() {
		auto model = std::make_shared<Model>();
		if (model->loadFromFile("assets/Untitled.obj")) {
			m_loadedModels["test"] = model;

			// Just print info for now
			std::cout << "Model loaded successfully!" << std::endl;
			std::cout << "Memory: " << model->getTotalMemoryUsage() / 1024 << " KB" << std::endl;
			std::cout << "Meshes: " << model->getMeshes().size() << std::endl;
		}
	}
};