#include "Engine/Utils/pch.h"
#include "Light.h"

#include <GL/glew.h>

#include "Engine/Scene/World.h"

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
	m_OuterCutOff(0.0f),
	m_FBO(nullptr)
{
}

Light::~Light()
{
	if(m_Depthmap != nullptr)
	{
		delete m_FBO;
		m_FBO = nullptr;
	}
	
}

void Light::CreateDirectionalDepthMap(const unsigned int& width, const unsigned int& height)
{
	if(m_FBO != nullptr)
	{
		delete m_FBO;
	}
	m_FBO = new Framebuffer();
	m_ShadowWidth = width;
	m_ShadowHeight = height;
	constexpr float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };

	m_FBO = new Framebuffer();
	m_ShadowWidth = width;
	m_ShadowHeight = height;
	m_Depthmap = CreateRef<Texture>(TextureTarget::Texture2D);
	m_Depthmap->SetTextureType(TextureType::DepthMap);
	m_Depthmap->SetTexture2DSize(m_ShadowWidth, m_ShadowHeight);
	m_Depthmap->CreateTextureCubeMapStorage(TextureInternalFormat::DepthComponent16);
	m_Depthmap->SetFilter(TextureFilter::Nearest, TextureFilter::Nearest);
	m_Depthmap->SetWrapMode(TextureWrap::ClampToBorder, TextureWrap::ClampToBorder);
	m_Depthmap->SetBorderColor(borderColor);
	m_Depthmap->SetUniformLocation("depthMap");

	m_FBO->AttachDepthTexture(m_Depthmap);
	m_FBO->SetDrawBuffer(FramebufferColorBuffer::None);
	m_FBO->SetDrawBuffer(FramebufferColorBuffer::None);
}

void Light::CreatePointDepthMap(const unsigned int& width, const unsigned int& height)
{
	if (m_FBO != nullptr)
	{
		delete m_FBO;
	}
	m_FBO = new Framebuffer();
	m_ShadowWidth = width;
	m_ShadowHeight = height;
	m_Depthmap = CreateRef<Texture>(TextureTarget::TextureCubeMap);
	m_Depthmap->SetTextureType(TextureType::DepthMap);
	m_Depthmap->SetTexture2DSize(m_ShadowWidth, m_ShadowHeight);
	m_Depthmap->CreateTextureCubeMapStorage(TextureInternalFormat::DepthComponent16);
	m_Depthmap->SetFilter(TextureFilter::Nearest, TextureFilter::Nearest);
	m_Depthmap->SetWrapMode(TextureWrap::ClampToEdge, TextureWrap::ClampToEdge, TextureWrap::ClampToEdge);
	m_Depthmap->SetUniformLocation("depthMap");

	m_FBO->AttachDepthTexture(m_Depthmap);
	m_FBO->SetDrawBuffer(FramebufferColorBuffer::None);
	m_FBO->SetDrawBuffer(FramebufferColorBuffer::None);
}

glm::mat4 Light::CreateLightSpaceMatrix(const float& nearPlane, const float& farPlane)
{
	m_NearPlane = nearPlane;
	m_FarPlane = farPlane;
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	//lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
	lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, m_NearPlane, m_FarPlane);
	lightView = glm::lookAt(m_Position, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;
	// render scene from light's point of view#
	return lightSpaceMatrix;
}

std::vector<glm::mat4> Light::CreateShadowTransformMatrices(const float& nearPlane, const float& farPlane)
{
	m_NearPlane = nearPlane;
	m_FarPlane = farPlane;
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)m_ShadowWidth / (float)m_ShadowHeight, m_NearPlane, m_FarPlane);
	std::vector<glm::mat4> shadowTransforms;
	shadowTransforms.push_back(shadowProj * glm::lookAt(m_Position, m_Position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(m_Position, m_Position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(m_Position, m_Position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(m_Position, m_Position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(m_Position, m_Position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(m_Position, m_Position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	return shadowTransforms;
}