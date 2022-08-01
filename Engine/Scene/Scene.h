#pragma once

#include "Animation.h"
#include "Camera.h"
#include "ReflectionProbe.h"
#include "SceneObject.h"
#include "../../Engine/Components/Light.h"

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

	ISceneObject* GetRootSceneNode();
	void AddRootChild(SceneObject* s);
	void RemoveRootChild(const std::string& name);
	ISceneObject* GetSceneObject(const std::string& name);
	std::unordered_map<std::string, ISceneObject*> GetSceneObjects();
	void AddSceneObject(ISceneObject*);

	void SetSceneCamera(Camera* s);
	Camera* GetSceneCamera();
	void RemoveSceneCamera();

	void SetSceneSkybox (Skybox* s);
	Skybox* GetSceneSkybox();
	void RemoveSceneSkybox();

	void SetReflectionProbe(ReflectionProbe* rp);
	ReflectionProbe* GetReflectionProbe();
	
private:
	std::string m_Name;

	Camera* m_SceneCamera;
	Skybox* m_Skybox = nullptr;

	ISceneObject* m_RootSO;
	
	std::unordered_map<std::string, ISceneObject*> m_SceneObjects;
	std::unordered_map<std::string, Light*> m_LightSources;

	ReflectionProbe* m_ReflectionProbe;
};
