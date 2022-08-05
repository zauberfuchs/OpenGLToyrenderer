#pragma once

#include "Scene.h"
#include "../../Engine/OpenGL/Shader.h"
#include "../../Engine/Interfaces/IMesh.h"

class Window;

class World
{
public:
	World(const World&) = delete;
	World& operator=(const World&) = delete;
	World(World&&) = delete;
	World& operator=(World&&) = delete;

	void AddShader(Shader* shader);
	Shader* GetShader(const std::string& name);

	void AddMaterial(IMaterial* m);
	IMaterial* GetMaterial(const std::string& name);
	IMaterial* GetMaterial(const int& index);

	void SetActiveScene(Scene* s);
	Scene* GetActiveScene();

	void SetActiveWindow(Window* w) { m_Window = w; }
	Window* GetActiveWindow() { return m_Window; };

	static auto& Get() {
		static World world;
		return world;
	}

private:
	World();
	virtual ~World();

	std::unordered_map<std::string, IMaterial*> m_MaterialCache;
	std::unordered_map<std::string, Shader*> m_ShaderCache;

	Window* m_Window;
	Scene* m_ActiveScene;
	glm::mat4 m_CoordinateSystem = glm::mat4(1.0f);
};
