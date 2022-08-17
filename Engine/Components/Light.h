#pragma once

#include "Engine/OpenGL/Framebuffer.h"
#include "Engine/OpenGL/Texture.h"

enum class LightSourceType
{
	PointLight, DirectionalLight, SpotLight
};

class Light
{
public:

	Light(const std::string& name);
	~Light();

	void CreateDirectionalDepthMap(const unsigned int& width, const unsigned int& height);
	void CreatePointDepthMap(const unsigned int& width, const unsigned int& height);

	std::vector<glm::mat4> CreateShadowTransformMatrices(const float& nearPlane, const float& farPlane);
	glm::mat4 CreateLightSpaceMatrix(const float& nearPlane, const float& farPlane);

	inline LightSourceType GetType() { return m_Type; }
	inline void SetType(const LightSourceType& type) { m_Type = type; }

	inline std::string GetName() { return m_Name; }
	inline void SetName(const std::string& name) { m_Name = name; }

	inline glm::vec3 GetDirection() { return m_Direction; }
	inline void SetDirection(const glm::vec3& direction) { m_Direction = direction; }

	inline glm::vec3 GetPosition() { return m_Position; }
	inline void SetPosition(const glm::vec3& position) { m_Position = position; }

	inline glm::vec3 GetColor() { return m_Color; }
	inline void SetColor(const glm::vec3& color) { m_Color = color; }

	inline float GetConstant() { return m_Constant; }
	inline void SetConstant(const float& constant) { m_Constant = constant; }

	inline float GetLinear() { return m_Linear; }
	inline void SetLinear(const float& linear) { m_Linear = linear; }

	inline float GetQuadratic() { return m_Quadratic; }
	inline void SetQuadratic(const float& quadratic) { m_Quadratic = quadratic; }

	inline float GetCutOff() { return m_CutOff; }
	inline void SetCutOff(const float& cutOff) { m_CutOff = cutOff; }

	inline float GetOuterCutOff() { return m_OuterCutOff; }
	inline void SetOuterCutOff(const float& outerCutOff) { m_OuterCutOff = outerCutOff; }

	inline int GetShadowWidth() { return m_ShadowWidth; }
	inline int GetShadowHeight() { return m_ShadowHeight; }

	inline Framebuffer* GetFramebuffer() { return m_FBO; }
	inline Texture* GetDepthmap() { return m_Depthmap; }


	inline float GetNearPlane() { return m_NearPlane; }
	inline void SetNearPlane(const float& nearPlane) { m_NearPlane = nearPlane; }

	inline float GetFarPlane() { return m_FarPlane; }
	inline void SetFarPlane(const float& farPlane) { m_FarPlane = farPlane; }

private:
	unsigned int m_ShadowWidth;
	unsigned int m_ShadowHeight;

	LightSourceType m_Type;

	glm::vec3 m_Direction;
	glm::vec3 m_Position;
	glm::vec3 m_Color;

	float m_Constant;
	float m_Linear;
	float m_Quadratic;

	float m_CutOff;
	float m_OuterCutOff;

	float m_NearPlane;
	float m_FarPlane;

	Framebuffer* m_FBO;
	Texture* m_Depthmap;


	std::string m_Name = "noname";
};