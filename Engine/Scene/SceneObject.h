#pragma once

#include "../../Engine/Interfaces/ISceneObject.h"

class SceneObject : public ISceneObject
{
public:
	SceneObject(std::string name);
	~SceneObject() override;

	std::string GetName() override;

	void AddModel(IModel* model) override;

	void AddLight(Light* light) override;
	Light* GetLight() override;
	
	void AddChildren(ISceneObject* sceneObject) override;
	std::unordered_set<ISceneObject*> GetChildren() override;

	void SetParent(ISceneObject* sceneObject) override;
	ISceneObject* GetParent() const override;
	
	Transform* GetTransform() override;

	Animation* GetAnimation() override;
	void SetAnimation(Animation* anim) override;

	IModel& GetModel() override;

private:
	std::string m_Name;

	ISceneObject* m_Parent = nullptr;
	std::unordered_set<ISceneObject*> m_Children;

	Transform m_Transform;

	Animation* m_Animation = nullptr;

	Light* m_Light = nullptr;
	IModel* m_Model = nullptr;
};