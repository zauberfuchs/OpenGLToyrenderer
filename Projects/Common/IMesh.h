#pragma once

#include <string>

#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Transform.h"
#include "Material.h"
#include "MaterialPBR.h"

enum class LightSourceType
{
	PointLight, DirectionalLight, SpotLight
};

struct Light 
{
	Light(const std::string name) :
		m_Name(name),
		Type(LightSourceType::DirectionalLight),
		Direction(glm::vec3(0.0f, 0.0f, 0.0f)),
		Position(glm::vec3(0.0f, 0.0f, 0.0f)),
		Color(glm::vec3(0.0f, 0.0f, 0.0f)),
		Constant(0.0f),
		Linear(0.0f),
		Quadratic(0.0f),
		CutOff(0.0f),
		OuterCutOff(0.0f)
	{
	};

	virtual ~Light() {};


	std::string GetName() { return m_Name; }

	LightSourceType Type;

	glm::vec3 Direction;
	glm::vec3 Position;
	glm::vec3 Color;

	float Constant;
	float Linear;
	float Quadratic;

	float CutOff;
	float OuterCutOff;

	std::string m_Name = "noname";
};

class IMesh
{
public:
	virtual ~IMesh() = default;

	virtual void SetupMesh() = 0;
	virtual void Draw(Transform& transform) = 0;

	virtual void SetName(const std::string& name) = 0;
	virtual std::string GetName() = 0;

	virtual void SetMaterial(IMaterial* material) = 0;
	virtual IMaterial* GetMaterial() = 0;
};