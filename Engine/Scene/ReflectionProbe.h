#pragma once


#include "Engine/OpenGL/Shader.h"
#include "Engine/OpenGL/Renderbuffer.h"
#include "Engine/OpenGL/Framebuffer.h"
#include "Engine/OpenGL/Texture.h"

class ReflectionProbe
{
public:
	ReflectionProbe(const int& width, const int& height);
	~ReflectionProbe() = default;

	void Create();
	void CreateReflectionMapFromHDR(const std::string& path);
	void SetReflectionMap(Ref<Texture> texture);

	inline Ref<Texture> GetIrradianceTexture() 	{ return m_IrradianceTexture; }
	inline Ref<Texture> GetPrefilterTexture() 	{ return m_PrefilterTexture; }
	inline Ref<Texture> GetBrdfLookUpTexture() 	{ return m_BrdfLookUpTexture; }
	inline Ref<Texture> GetReflectionTexture() 	{ return m_ReflectionTexture; }

private:

	void CreateIrradianceMap();
	void CreatePrefilterMap();
	void CreateBRDFLookUpTexture();
	
	Renderbuffer m_RBO;
	Framebuffer m_FBO;

	Ref<Texture> m_IrradianceTexture;
	Ref<Texture> m_PrefilterTexture;
	Ref<Texture> m_BrdfLookUpTexture;
	Ref<Texture> m_ReflectionTexture;

	Shader* m_BrdfShader;
	Shader* m_PrefilterShader;
	Shader* m_IrradianceShader;
	Shader* m_PbrShader;
	Shader* m_EquirectangularToCubemapShader;

	glm::mat4 m_CaptureProjection;
	glm::mat4 m_CaptureViews[6];
};
