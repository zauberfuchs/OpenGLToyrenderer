#pragma once

#include "Scene.h"
#include "../../Engine/OpenGL/Shader.h"

class Material;
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

	void AddMaterial(Material* m);
	Material* GetMaterial(const std::string& name);
	Material* GetMaterial(const int& index);
	std::unordered_map<std::string, Material*> GetMaterials() { return m_MaterialCache; }

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

	std::unordered_map<std::string, Material*> m_MaterialCache;
	std::unordered_map<std::string, Shader*> m_ShaderCache;

	Window* m_Window;
	Scene* m_ActiveScene;
	glm::mat4 m_CoordinateSystem = glm::mat4(1.0f);
};
