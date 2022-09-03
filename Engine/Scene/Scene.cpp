#include "Engine/Utils/pch.h"
#include "Scene.h"

Scene::Scene(const std::string& name) :
	m_Name(name),
	m_SceneCamera(nullptr),
	m_RootSO(CreateRef<SceneObject>("root"))
{
}

Scene::~Scene()
{
}

void Scene::UpdateScene()
{
}

void Scene::AddRootChild(Ref<SceneObject> s)
{
	m_RootSO->AddChildren(s);
	m_SceneObjects.insert({ s->GetName(), s });
}

void Scene::AddSceneObject(Ref<SceneObject> s)
{
	m_SceneObjects.insert({ s->GetName(), s });
}

void Scene::AddSceneLight(Light* l)
{
	m_LightSources.insert( {l->GetName(), l} );
}

void Scene::RemoveSceneLight(const std::string& name)
{
	m_LightSources.erase("name");
}

void Scene::RemoveRootChild(const std::string& name)
{
	m_SceneObjects.erase("name");
}

void Scene::SetSceneSkybox(Texture* s)
{
	m_Skybox = s;
}

Texture* Scene::GetSceneSkybox()
{
	return m_Skybox;
}

void Scene::SetSceneCamera(Ref<Camera> c)
{
	m_SceneCamera = c;
}

void Scene::SetReflectionProbe(ReflectionProbe* rp)
{
	m_ReflectionProbe = rp;
}

ReflectionProbe* Scene::GetReflectionProbe()
{
	return m_ReflectionProbe;
}


std::unordered_map<std::string, Ref<SceneObject>> Scene::GetSceneObjects()
{
	return m_SceneObjects;
}

Ref<SceneObject> Scene::GetSceneObject(const std::string& name)
{
	return m_SceneObjects.at(name);
}

std::unordered_map<std::string, Light*> Scene::GetSceneLightSources()
{
	return m_LightSources;
}

Ref<SceneObject> Scene::GetRootSceneNode()
{
	return m_RootSO;
}

Ref<Camera> Scene::GetSceneCamera()
{
	return m_SceneCamera;
}

void Scene::RemoveSceneCamera()
{
	m_SceneCamera = nullptr;
}

void Scene::RemoveSceneSkybox()
{
	m_Skybox = nullptr;
}
