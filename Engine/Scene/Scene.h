#pragma once


#include "Camera.h"
#include "ReflectionProbe.h"
#include "SceneObject.h"
#include "Engine/Components/Light.h"

class Skybox;

class Scene
{
public:
	Scene(const std::string& name);
	~Scene();
	
	void UpdateScene();

	void AddSceneLight(Light* l);
	void RemoveSceneLight(const std::string& name);
	std::unordered_map<std::string, Light*> GetSceneLightSources();

	Ref<SceneObject> GetRootSceneNode();
	void AddRootChild(Ref<SceneObject> s);
	void RemoveRootChild(const std::string& name);

	Ref<SceneObject> GetSceneObject(const std::string& name);
	std::unordered_map<std::string, Ref<SceneObject>> GetSceneObjects();
	void AddSceneObject(Ref<SceneObject>);

	void SetSceneCamera(Ref<Camera> s);
	Ref<Camera> GetSceneCamera();
	void RemoveSceneCamera();

	void SetSceneSkybox (Texture* s);
	Texture* GetSceneSkybox();
	void RemoveSceneSkybox();

	void SetReflectionProbe(ReflectionProbe* rp);
	ReflectionProbe* GetReflectionProbe();
	
private:
	std::string m_Name;

	Ref<Camera> m_SceneCamera;
	Texture* m_Skybox = nullptr;

	Ref<SceneObject> m_RootSO = CreateRef<SceneObject>("root");
	
	std::unordered_map<std::string, Ref<SceneObject>> m_SceneObjects;
	std::unordered_map<std::string, Light*> m_LightSources;

	ReflectionProbe* m_ReflectionProbe;
};
