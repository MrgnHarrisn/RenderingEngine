#pragma once

#include <cerrno>
#include <glm/glm.hpp>

#include "Core/Window.h"
#include "Renderer/Model.h"
#include "Renderer/Shader.h"

class Renderer
{
public:

	/// <summary>
	/// Create renderer and attach it to a window
	/// </summary>
	/// <param name="target">Target window to render to</param>
	Renderer(Window* target);

	/// <summary>
	/// Set the clear color
	/// </summary>
	/// <param name="color">Color</param>
	void setClearColor(glm::vec3 color);

	/// <summary>
	/// Initialize the renderer
	/// </summary>
	/// <returns></returns>
	bool init();

	/// <summary>
	/// Render something
	/// </summary>
	void render();

	/// <summary>
	/// Enable/disable wireframe mode
	/// </summary>
	/// <param name="enable">True for wireframe, false for solid</param>
	void setWireframeMode(bool enable);

	/// <summary>
	/// Get render statistics
	/// </summary>
	struct RenderStats {
		int drawCalls = 0;
		int trianglesDrawn = 0;
		int verticesDrawn = 0;
	};

	/// <summary>
	/// 
	/// </summary>
	void beginFrame();

	/// <summary>
	/// 
	/// </summary>
	void endFrame();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="model"></param>
	/// <param name="transform"></param>
	void renderModel(Model& model, const glm::mat4& transform);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="mesh"></param>
	/// <param name="transform"></param>
	void renderMesh(Mesh& mesh, const glm::mat4& transform);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="view"></param>
	void setViewMatrix(const glm::mat4& view);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="projection"></param>
	void Renderer::setProjectionMatrix(const glm::mat4& projection);


	/// <summary>
	/// 
	/// </summary>
	/// <param name="enable"></param>
	void Renderer::setDepthTesting(bool enable);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="enable"></param>
	void Renderer::setBackfaceCulling(bool enable);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="width"></param>
	/// <param name="height"></param>
	void setViewport(int width, int height);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="color"></param>
	void setClearColor(glm::vec4 color);

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	const RenderStats& getStats() const { return m_stats; }

	/// <summary>
	/// 
	/// </summary>
	void resetStats() { m_stats = RenderStats(); }

	~Renderer();

private:
	/// <summary>
	/// Attach renderer to the window
	/// </summary>
	/// <param name="target">Window to render to</param>
	void attach(Window* target);

	/// <summary>
	/// Setup OpenGL state
	/// </summary>
	void setupOpenGLState();

	// Render statistics
	RenderStats m_stats;

	// Render settings
	glm::vec4 m_clearColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	bool m_depthTesting = true;
	bool m_backfaceCulling = true;
	bool m_wireframeMode = false;

	Window* m_target = nullptr;
	Shader m_defaultShader;
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;
	bool m_initialized = false;

	// For error checking
	void checkGLError(const char* function);
};