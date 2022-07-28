#include "../../Engine/Utils/pch.h"
#include "World.h"

World::World()
{
}

World::~World()
{
}


void World::AddShader(Shader* shader)
{
	m_ShaderCache.insert({ shader->GetName(), shader });
}

Shader* World::GetShader(const std::string& name)
{
	return m_ShaderCache.at(name);
}

void World::AddMaterial(IMaterial* m)
{
	m_MaterialCache.insert({ m->GetName(), m });
}

IMaterial* World::GetMaterial(const std::string& name)
{
	return m_MaterialCache.at(name);
}

IMaterial* World::GetMaterial(const int& index)
{
	auto it = m_MaterialCache.begin();
	std::advance(it, index);
	return it->second;
}

void World::SetActiveScene(Scene* s)
{
	m_ActiveScene = s;
}

Scene* World::GetActiveScene()
{
	return m_ActiveScene;
}


