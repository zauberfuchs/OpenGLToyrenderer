#pragma once


#include "Engine/OpenGL/Shader.h"
#include "Engine/OpenGL/Renderbuffer.h"
#include "Engine/OpenGL/Framebuffer.h"
#include "Engine/OpenGL/Texture.h"

class Mesh;

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

	inline Texture* GetIrradianceTexture() { return &m_IrradianceTexture; }
	inline Texture* GetPrefilterTexture() { return &m_PrefilterTexture; }
	inline Texture* GetBrdfLookUpTexture() { return &m_BrdfLookUpTexture; }

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

	Mesh* m_Cube;

	unsigned int m_ReflectionMap;
	unsigned int m_IrradianceMap;
	unsigned int m_PrefilterMap;
	unsigned int m_BrdfLUT;

	Texture m_IrradianceTexture;
	Texture m_PrefilterTexture;
	Texture m_BrdfLookUpTexture;
	Texture m_ReflectionTexture;

	Shader* m_BrdfShader;
	Shader* m_PrefilterShader;
	Shader* m_IrradianceShader;
	Shader* m_PbrShader;
	Shader* m_EquirectangularToCubemapShader;

	glm::mat4 m_CaptureProjection;
	glm::mat4 m_CaptureViews[6];
};
