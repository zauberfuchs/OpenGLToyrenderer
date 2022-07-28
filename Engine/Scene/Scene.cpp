#include "../../Engine/Utils/pch.h"
#include "Scene.h"

#include "Skybox.h"
#include "World.h"

Scene::Scene(const std::string& name) :
	m_Name(name),
	m_SceneCamera(nullptr),
	m_RootSO(new SceneObject("root"))
{
}

Scene::~Scene()
{
	if (m_RootSO != nullptr) {
		delete m_RootSO;
		m_RootSO = nullptr;
	}
}

void Scene::UpdateScene()
{
	RenderDepthMap();
}

void Scene::RenderScene()
{

	if (m_Skybox != nullptr) {
		m_Skybox->Render();
	}
	m_RootSO->Render();
}

void Scene::RenderDepthMap()
{
	for (auto l: m_LightSources)
	{
		l.second->CreateDepthMap(1024, 1024);
		
		glViewport(0, 0, l.second->m_ShadowWidth, l.second->m_ShadowHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, l.second->m_FBO.GetId());
		glClear(GL_DEPTH_BUFFER_BIT);
		World::Get().GetShader("shadowmapShader")->Bind();
		World::Get().GetShader("shadowmapShader")->SetUniformMat4f("lightSpaceMatrix", l.second->CreateLightSpaceMatrix());
		RenderScene();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}


void Scene::AddRootChild(SceneObject* s)
{
	m_RootSO->AddChildren(s);
	m_SceneObjects.insert({ s->GetName(), s });
}

//Todo ?? redundant
void Scene::AddSceneObject(ISceneObject* s)
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

void Scene::SetSceneSkybox(Skybox* s)
{
	m_Skybox = s;
}

Skybox* Scene::GetSceneSkybox()
{
	return m_Skybox;
}

void Scene::SetSceneCamera(Camera* c)
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


std::unordered_map<std::string, ISceneObject*> Scene::GetSceneObjects()
{
	return m_SceneObjects;
}

ISceneObject* Scene::GetSceneObject(const std::string& name)
{
	return m_SceneObjects.at(name);
}

std::unordered_map<std::string, Light*> Scene::GetSceneLightSources()
{
	return m_LightSources;
}

ISceneObject* Scene::GetRootSceneNode()
{
	return m_RootSO;
}

Camera* Scene::GetSceneCamera()
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
