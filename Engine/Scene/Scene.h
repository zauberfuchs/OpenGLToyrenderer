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


	Ref<SceneObject> GetRootSceneNode();
	void AddRootChild(Ref<SceneObject> s);
	void RemoveRootChild(const std::string& name);

	Ref<SceneObject> GetSceneObject(const std::string& name);
	std::unordered_map<std::string, Ref<SceneObject>> GetSceneObjects();
	void AddSceneObject(Ref<SceneObject>);

	void SetSceneCamera(Ref<Camera> s);
	Ref<Camera> GetSceneCamera();
	void RemoveSceneCamera();

	void SetSceneSkybox (Ref<Texture> s);
	Ref<Texture> GetSceneSkybox();
	void RemoveSceneSkybox();

	void SetReflectionProbe(Ref<ReflectionProbe> rp);
	Ref<ReflectionProbe> GetReflectionProbe();
	
private:
	std::string m_Name;

	Ref<Camera> m_SceneCamera;
	Ref<Texture> m_Skybox = nullptr;

	Ref<SceneObject> m_RootSO = CreateRef<SceneObject>("root");
	
	std::unordered_map<std::string, Ref<SceneObject>> m_SceneObjects;

	Ref<ReflectionProbe> m_ReflectionProbe;
};
