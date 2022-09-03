#pragma once

#include "Scene.h"
#include "Engine/OpenGL/Shader.h"

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

	void AddMaterial(Ref<Material> m);
	Ref<Material> GetMaterial(const std::string& name);
	Ref<Material> GetMaterial(const int& index);
	std::unordered_map<std::string, Ref<Material>> GetMaterials() { return m_MaterialCache; }

	void SetActiveScene(Ref<Scene> s);
	Ref<Scene> GetActiveScene();

	void SetActiveWindow(Ref<Window> w) { m_Window = w; }
	Ref<Window> GetActiveWindow() { return m_Window; };

	static auto& Get() {
		static World world;
		return world;
	}

private:
	World();
	virtual ~World();

	std::unordered_map<std::string, Ref<Material>> m_MaterialCache;
	std::unordered_map<std::string, Shader*> m_ShaderCache;

	Ref<Window> m_Window;
	Ref<Scene> m_ActiveScene;
	glm::mat4 m_CoordinateSystem = glm::mat4(1.0f);
};
