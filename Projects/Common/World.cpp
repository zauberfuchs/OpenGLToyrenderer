#include "World.h"

World::World()
{
}

World::~World()
{
}

void World::UpdateWorld()
{
	m_ActiveScene->UpdateScene();
}

void World::RenderWorld()
{
	m_ActiveScene->RenderScene();
}

void World::AddShader(Shader* shader)
{
	m_ShaderCache.insert({ shader->GetName(), shader });
}

Shader* World::GetShader(const std::string& name)
{
	return m_ShaderCache.at(name);
}

void World::SetActiveScene(Scene* s)
{
	m_ActiveScene = s;
}

Scene* World::GetActiveScene()
{
	return m_ActiveScene;
}
