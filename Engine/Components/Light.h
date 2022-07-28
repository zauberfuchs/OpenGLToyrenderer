#pragma once

#include <GL/glew.h>
#include "../../Engine/OpenGL/Framebuffer.h"
#include "../../Engine/OpenGL/Texture.h"

enum class LightSourceType
{
	PointLight, DirectionalLight, SpotLight
};

//TOdo make class
struct Light
{
	Light(const std::string& name);
	virtual ~Light() = default;

	void CreateDepthMap(const unsigned int& width, const unsigned int& height);

	glm::mat4 CreateLightSpaceMatrix();


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