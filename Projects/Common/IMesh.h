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

//TOdo make class
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
	//Todo eigene klassen nutzen wie Texture??
	void CreateDepthMap(const unsigned int& width, const unsigned int& height)
	{
		//Todo create depthmap texture in texture class
		m_ShadowWidth = width;
		m_ShadowHeight = height;
		unsigned int depthMap;
		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			m_ShadowWidth, m_ShadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO.GetId());
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		m_DepthMap.m_ID = depthMap;
		m_DepthMap.SetTextureType(ETextureChannels::DepthMap);
	}

	glm::mat4 CreateLightSpaceMatrix()
	{
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 1.0f, far_plane = 7.5f;
		//lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(Position, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		// render scene from light's point of view#
		return lightSpaceMatrix;
	}


	unsigned int m_ShadowWidth;
	unsigned int m_ShadowHeight;

	std::string GetName() { return m_Name; }
	Framebuffer GetFramebuffer() { return m_FBO; }

	LightSourceType Type;

	glm::vec3 Direction;
	glm::vec3 Position;
	glm::vec3 Color;

	float Constant;
	float Linear;
	float Quadratic;

	float CutOff;
	float OuterCutOff;

	Framebuffer m_FBO;
	Texture m_DepthMap;


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