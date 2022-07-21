#pragma once

#include <unordered_map>

#include "Scene.h"
#include "Shader.h"

class World
{
public:
	World(const World&) = delete;
	World& operator=(const World&) = delete;
	World(World&&) = delete;
	World& operator=(World&&) = delete;

	void RenderWorld();
	void UpdateWorld();

	void AddShader(Shader* shader);
	Shader* GetShader(const std::string& name);

	void SetActiveScene(Scene* s);
	Scene* GetActiveScene();
	

	static auto& Get() {
		static World world;
		return world;
	}

private:
	World();
	virtual ~World();

	std::unordered_map<std::string, Shader*> m_ShaderCache;

	Scene* m_ActiveScene;
	glm::mat4 m_CoordinateSystem = glm::mat4(1.0f);
};
