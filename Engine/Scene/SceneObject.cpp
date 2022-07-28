#include "../../Engine/Utils/pch.h"
#include "../../Engine/Scene/SceneObject.h"

#include "World.h"


SceneObject::SceneObject(std::string name)
	: m_Name(name)
{
}

SceneObject::~SceneObject()
{
	delete m_Model;
}

void SceneObject::Render() 
{

	if (m_Model != nullptr) {
		m_Model->Draw(m_Transform);
	}
	std::unordered_set<ISceneObject*>::iterator iter = m_Children.begin();
	for (; iter != m_Children.end(); ++iter) {
		SceneObject* child = (SceneObject*) (*iter);
		if (child != nullptr) {
			child->Render();
		}
	}

	// alternativ
	/*for (auto c : m_Children) {
		c->Render();
	}*/
};

void SceneObject::AddModel(IModel* model)
{
	m_Model = model;
};

void SceneObject::AddLight(Light* light)
{
	m_Light = light;
}

Light* SceneObject::GetLight()
{
	return m_Light;
}

void SceneObject::AddChildren(ISceneObject* sceneObject)
{
	World::Get().GetActiveScene()->AddSceneObject(sceneObject);
	m_Children.insert(sceneObject);
	sceneObject->GetTransform()->SetParent(this->GetTransform());
	sceneObject->SetParent(this);
}

std::unordered_set<ISceneObject*> SceneObject::GetChildren()
{
	return m_Children;
}

void SceneObject::SetParent(ISceneObject* sceneObject)
{	
	m_Parent = sceneObject;
}

ISceneObject* SceneObject::GetParent() const
{
	return m_Parent;
}

std::string SceneObject::GetName()
{
	return m_Name;
}


IModel& SceneObject::GetModel()
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
	return &m_Transform;
}