#include "Engine/Utils/pch.h"
#include "Engine/Scene/SceneObject.h"

#include "World.h"
#include "Engine/Components/Model.h"


SceneObject::SceneObject(std::string name)
	: m_Name(name), m_Entity(World::Get().GetEntityManager()->create_entity())
{
	m_Entity.AddComponent<Transform>();
}

SceneObject::~SceneObject()
{
}

void SceneObject::AddChildren(Ref<SceneObject> sceneObject)
{
	World::Get().GetActiveScene()->AddSceneObject(sceneObject);
	m_Children.insert(sceneObject);
	sceneObject->GetComponent<Transform>().SetParent(&this->GetComponent<Transform>());
	sceneObject->SetParent(this);
}

std::unordered_set<Ref<SceneObject>> SceneObject::GetChildren()
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

Animation* SceneObject::GetAnimation()
{
	return m_Animation;
}

void SceneObject::SetAnimation(Animation* anim)
{
	m_Animation = anim;
}
