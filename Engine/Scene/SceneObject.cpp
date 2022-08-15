#include "Engine/Utils/pch.h"
#include "Engine/Scene/SceneObject.h"

#include "World.h"
#include "Engine/Components/Model.h"


SceneObject::SceneObject(std::string name)
	: m_Name(name), m_Transform(new Transform())
{
}

SceneObject::~SceneObject()
{
	delete m_Model;
}

void SceneObject::AddModel(Model* model)
{
	m_Model = model;
}

void SceneObject::AddLight(Light* light)
{
	m_Light = light;
}

Light* SceneObject::GetLight()
{
	return m_Light;
}

void SceneObject::AddChildren(SceneObject* sceneObject)
{
	World::Get().GetActiveScene()->AddSceneObject(sceneObject);
	m_Children.insert(sceneObject);
	sceneObject->GetTransform()->SetParent(this->GetTransform());
	sceneObject->SetParent(this);
}

std::unordered_set<SceneObject*> SceneObject::GetChildren()
{
	return m_Children;
}

void SceneObject::SetParent(SceneObject* sceneObject)
{	
	m_Parent = sceneObject;
}

SceneObject* SceneObject::GetParent() const
{
	return m_Parent;
}

std::string SceneObject::GetName()
{
	return m_Name;
}


Model& SceneObject::GetModel()
{
	return *m_Model;
}

Animation* SceneObject::GetAnimation()
{
	return m_Animation;
}

void SceneObject::SetAnimation(Animation* anim)
{
	m_Animation = anim;
}


Transform* SceneObject::GetTransform()
{
	return m_Transform;
}