#include "../../Engine/Utils/pch.h"
#include "Light.h"

#include "../../Engine/Scene/World.h"

Light::Light(const std::string& name) :
	m_Name(name),
	m_Type(LightSourceType::DirectionalLight),
	m_Direction(glm::vec3(0.0f, 0.0f, 0.0f)),
	m_Position(glm::vec3(0.0f, 0.0f, 0.0f)),
	m_Color(glm::vec3(0.0f, 0.0f, 0.0f)),
	m_Constant(0.0f),
	m_Linear(0.0f),
	m_Quadratic(0.0f),
	m_CutOff(0.0f),
	m_OuterCutOff(0.0f)
{
}

//Todo eigene klassen nutzen wie Texture??
void Light::CreateDepthMap(const unsigned int& width, const unsigned int& height)
{
	m_FBO = new Framebuffer();
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO->GetId());
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_Depthmap = new Texture();
	m_Depthmap->m_ID = depthMap;
	m_Depthmap->SetTextureType(ETextureChannels::DepthMap);

	//Todo wrong place??
	//World::Get().GetMaterial("pbrTextureShader")->SetTexture(m_Depthmap);
}

glm::mat4 Light::CreateLightSpaceMatrix()
{
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = 1.0f, far_plane = 7.5f;
	//lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	lightView = glm::lookAt(m_Position, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;
	// render scene from light's point of view#
	return lightSpaceMatrix;
}