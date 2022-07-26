#pragma once

#include <map>
#include <unordered_map>

#include "Animation.h"
#include "SceneObject.h"

class Skybox;

class Scene
{
public:
	Scene(const std::string& name);
	~Scene();

	void RenderScene();
	void RenderDepthMap();
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
	
private:
	std::string m_Name;

	Camera* m_SceneCamera;
	Skybox* m_Skybox = nullptr;

	ISceneObject* m_RootSO;
	
	std::unordered_map<std::string, ISceneObject*> m_SceneObjects;
	std::unordered_map<std::string, Light*> m_LightSources;

	
};
