#include "Transform.h"

#include <iostream>
#include <vector>



Transform::Transform() :
	m_WorldPosition(glm::vec3(0.0f)),
	m_WorldEulerAngles(glm::vec3(0.0f)),
	m_WorldScale(glm::vec3(1.0f)),
	m_LocalPosition(glm::vec3(0.0f)),
	m_LocalEulerAngles(glm::vec3(0.0f)),
	m_LocalScale(glm::vec3(1.0f)),
	m_Axis(glm::mat4(1.0f)),
	m_TransformationMatrix(glm::mat4(1.0f)),
	m_TransforMatrixSet(false)
{
}

void Transform::Translate(glm::vec3 translation, Space relativeTo)
{
	switch(relativeTo)
	{
	case Space::Local: m_LocalPosition = translation; break;
	case Space::World: m_WorldPosition = translation; break;
	}
}

void Transform::Rotate(glm::vec3 eulers, Space relativeTo)
{
	switch(relativeTo)
	{
	case Space::Local: m_LocalEulerAngles = m_LocalEulerAngles + eulers; break;
	case Space::World: m_WorldEulerAngles = m_WorldEulerAngles + eulers; break;
	}
}

void Transform::Scale(glm::vec3 scale, Space relativeTo)
{
	switch (relativeTo)
	{
	case Space::Local: m_LocalScale = scale; break;
	case Space::World: m_WorldScale = scale; break;
	}
}

void Transform::SetRotation(glm::vec3 eulers, Space relativeTo)
{
	switch (relativeTo)
	{
	case Space::Local: m_LocalEulerAngles = eulers; break;
	case Space::World: m_WorldEulerAngles = eulers; break;
	}
}

void Transform::SetConstraints(const std::initializer_list<Inheritance>& inheritance)
{
 	for (const auto i : inheritance)
	{
		switch (i)
		{
		case Inheritance::Translation: m_Constraints[0] = true; break;
		case Inheritance::Rotation: m_Constraints[1] = true; break;
		case Inheritance::Scale: m_Constraints[2] = true; break;
		}
	}
}

bool* Transform::GetConstraints()
{
	return m_Constraints;
}

void Transform::SetTransformMatrix(glm::mat4 matrix)
{
	m_LocalPosition = matrix[3];

	const float sx = glm::length(matrix[0]);
	const float sy = glm::length(matrix[1]);
	const float sz = glm::length(matrix[2]);
	m_LocalScale = glm::vec3(sx, sy, sz);

	glm::mat4 rotM = { matrix[1] / sx, matrix[2] / sy, matrix[3] / sz , {0, 0, 0, 1} };
	const float rx = glm::degrees(glm::asin(glm::abs(rotM[1].y)));
	const float ry = glm::degrees(glm::asin(glm::abs(rotM[1].x)));
	const float rz = glm::degrees(glm::asin(glm::abs(rotM[0].x)));
	m_LocalEulerAngles = glm::vec3(rx, ry, rz);
}

glm::mat4 Transform::GetParentMatrix()
{
	glm::mat4 m(1.0f);
	m = m * glm::translate(glm::mat4(1.0f), m_WorldPosition);
	m = m * glm::eulerAngleXYZ(glm::radians(m_WorldEulerAngles.x), glm::radians(m_WorldEulerAngles.y), glm::radians(m_WorldEulerAngles.z));
	m = m * glm::scale(m_WorldScale);

	glm::mat4 parentMatrix(1.0f);
	if (m_Parent != nullptr)
		parentMatrix = m_Parent->GetConstrainedTransform(m_Constraints) * m;

	return parentMatrix;
}

glm::mat4 Transform::GetConstrainedTransform(const bool* constraints)
{
	glm::mat4 m(1.0f);
	if (constraints[0])
		m = m * translate(glm::mat4(1.0f), m_LocalPosition);
	if (constraints[1])
		m = m * m_Axis * glm::eulerAngleXYZ(glm::radians(m_LocalEulerAngles.x), glm::radians(m_LocalEulerAngles.y), glm::radians(m_LocalEulerAngles.z));
	if (constraints[2])
		m = m * glm::scale(m_LocalScale);

	return GetParentMatrix() * m;

}

glm::mat4 Transform::GetTransformMatrix()
{
	m_TransformationMatrix = glm::translate(glm::mat4(1.0f), GetLocalPosition())
							* glm::eulerAngleXYZ(glm::radians(m_LocalEulerAngles.x), glm::radians(m_LocalEulerAngles.y), glm::radians(m_LocalEulerAngles.z))
							* glm::scale(glm::mat4(1.0f), GetLocalScale());

	return  GetParentMatrix() * m_Axis * m_TransformationMatrix;
}

void Transform::RotateAxis(glm::vec3 axis)
{
	m_Axis = glm::orientate4(axis);
}


void Transform::SetParent(Transform* p)
{
	m_Parent = p;
}


glm::vec3 Transform::GetLocalEulerAngles()
{
	return m_LocalEulerAngles;
}

glm::vec3 Transform::GetLocalPosition()
{
	return m_LocalPosition;
}

glm::vec3 Transform::GetLocalScale()
{
	return m_LocalScale;
}

glm::vec3 Transform::GetWorldEulerAngles()
{
	return m_WorldEulerAngles;
}

glm::vec3 Transform::GetWorldPosition()
{
	return m_WorldPosition;
}

glm::vec3 Transform::GetWorldScale()
{
	return m_WorldScale;
}

