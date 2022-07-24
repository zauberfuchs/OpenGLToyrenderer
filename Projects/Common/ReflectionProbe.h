#pragma once


#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Shader.h"
#include "Renderbuffer.h"
#include "Framebuffer.h"
#include "Texture.h"

class IMesh;

class ReflectionProbe
{
public:
	ReflectionProbe(const int& width, const int& height);
	~ReflectionProbe() = default;

	void Create();
	void CreateReflectionMapFromHDR(const std::string& path);
	void SetReflectionMap(const unsigned int& id);

	inline unsigned int GetIrradianceMap() { return m_IrradianceMap; }
	inline unsigned int GetPreFilterMap() { return m_PrefilterMap; }
	inline unsigned int GetBrdfLUT() { return m_BrdfLUT; }
	inline unsigned int GetReflectionMap() { return m_ReflectionMap; }

	inline ITexture* GetIrradianceTexture() { return &m_IrradianceTexture; }
	inline ITexture* GetPrefilterTexture() { return &m_PrefilterTexture; }
	inline ITexture* GetBrdfLookUpTexture() { return &m_BrdfLookUpTexture; }

private:

	void CreateIrradianceMap();
	void CreatePrefilterMap();
	void CreateBRDFLookUpTexture();
	void RenderCube();
	void RenderQuad();

	unsigned int m_QuadVAO;
	unsigned int m_QuadVBO;
	unsigned int m_CubeVAO;
	unsigned int m_CubeVBO;
	Renderbuffer m_RBO;
	Framebuffer m_FBO;

	IMesh* m_Cube;

	unsigned int m_ReflectionMap;
	unsigned int m_IrradianceMap;
	unsigned int m_PrefilterMap;
	unsigned int m_BrdfLUT;

	Texture m_IrradianceTexture;
	Texture m_PrefilterTexture;
	Texture m_BrdfLookUpTexture;

	Shader* m_BrdfShader;
	Shader* m_PrefilterShader;
	Shader* m_IrradianceShader;
	Shader* m_PbrShader;
	Shader* m_EquirectangularToCubemapShader;

	glm::mat4 m_CaptureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 m_CaptureViews[6] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};
};
