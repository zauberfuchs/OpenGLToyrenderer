#include "Engine/Utils/pch.h"
#include "ReflectionProbe.h"

#include "Engine/Renderer/Renderer.h"
#include "World.h"


ReflectionProbe::ReflectionProbe(const int& width, const int& height)
	: m_BrdfLookUpTexture(CreateRef<Texture>(TextureTarget::Texture2D)),
		m_IrradianceTexture(CreateRef<Texture>(TextureTarget::TextureCubeMap)),
		m_PrefilterTexture(CreateRef<Texture>(TextureTarget::TextureCubeMap)),
		m_ReflectionTexture(CreateRef<Texture>(TextureTarget::TextureCubeMap)),
		m_CaptureProjection(glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f)),
		m_CaptureViews{ glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
						glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
						glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
						glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
						glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
						glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)) }
{
	m_IrradianceShader = World::Get().GetShader("irradianceConvolution");
	m_PrefilterShader = World::Get().GetShader("prefilter");
	m_BrdfShader = World::Get().GetShader("brdf");
	m_EquirectangularToCubemapShader = World::Get().GetShader("equirectangularToCubemap");
}

void ReflectionProbe::Create()
{
    m_FBO.Bind();
	CreateIrradianceMap();
	CreatePrefilterMap();
	CreateBRDFLookUpTexture();
	m_FBO.Unbind();
}

void ReflectionProbe::CreateReflectionMapFromHDR(const std::string& path)
{
    m_FBO.Bind();
    m_RBO.CreateRenderBufferStorage(512, 512, FramebufferTextureFormat::Depth24);
    m_FBO.AttachRenderBuffer(m_RBO.GetId(), FramebufferAttachment::Depth);

    // load the HDR environment map
    Texture envHDRMap(TextureTarget::Texture2D);
    envHDRMap.Load(path, TextureWrap::ClampToEdge, TextureFilter::Linear);
    envHDRMap.Bind(0);

    // setup cubemap to render to and attach to framebuffer
    m_ReflectionTexture->SetTexture2DSize(512, 512);
    m_ReflectionTexture->CreateTextureCubeMapStorage(TextureInternalFormat::Rgb16F);
    m_ReflectionTexture->SetWrapMode(TextureWrap::ClampToEdge, TextureWrap::ClampToEdge, TextureWrap::ClampToEdge);
    m_ReflectionTexture->SetFilter(TextureFilter::Linear, TextureFilter::Linear);

    // convert HDR equirectangular environment map to cubemap equivalent
    m_EquirectangularToCubemapShader->Bind();
    m_EquirectangularToCubemapShader->SetUniform1i("equirectangularMap", 0);
    m_EquirectangularToCubemapShader->SetUniformMat4f("projection", m_CaptureProjection);

    glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
    for (unsigned int i = 0; i < 6; ++i)
    {
        m_EquirectangularToCubemapShader->SetUniformMat4f("view", m_CaptureViews[i]);
        m_FBO.AttachColorTexture3D(i, m_ReflectionTexture);
        Renderer::RenderCube();
    }

    m_FBO.Unbind();
    m_EquirectangularToCubemapShader->Unbind();
}

void ReflectionProbe::SetReflectionMap(Ref<Texture> texture)
{
    m_ReflectionTexture = texture;
}

void ReflectionProbe::CreateIrradianceMap()
{
    m_IrradianceTexture->SetTexture2DSize(32, 32);
    m_IrradianceTexture->CreateTextureCubeMapStorage(TextureInternalFormat::Rgb16F);
    m_IrradianceTexture->SetWrapMode(TextureWrap::ClampToEdge, TextureWrap::ClampToEdge, TextureWrap::ClampToEdge);
    m_IrradianceTexture->SetFilter(TextureFilter::Linear, TextureFilter::Linear);
    m_IrradianceTexture->SetTextureType(TextureType::IrradianceMap);
    m_IrradianceTexture->SetUniformLocation("material.irradianceMap");

    m_RBO.CreateRenderBufferStorage(32, 32, FramebufferTextureFormat::Depth24);

    // pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
    m_IrradianceShader->Bind();
    m_IrradianceShader->SetUniform1i("environmentMap", 0);
    m_IrradianceShader->SetUniformMat4f("projection", m_CaptureProjection);

    m_ReflectionTexture->Bind(0);
    
    glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
    for (unsigned int i = 0; i < 6; ++i)
    {
        m_IrradianceShader->SetUniformMat4f("view", m_CaptureViews[i]);
        m_FBO.AttachColorTexture3D(i, m_IrradianceTexture);
        Renderer::RenderCube();
    }
    
    m_IrradianceShader->Unbind();
}


void ReflectionProbe::CreatePrefilterMap()
{
    m_PrefilterTexture->SetTexture2DSize(128, 128);
    m_PrefilterTexture->CreateTextureCubeMapStorage(TextureInternalFormat::Rgb16F, true);
    m_PrefilterTexture->SetWrapMode(TextureWrap::ClampToEdge, TextureWrap::ClampToEdge, TextureWrap::ClampToEdge);
    m_PrefilterTexture->SetFilter(TextureFilter::MipMapLinear, TextureFilter::Linear);
    m_PrefilterTexture->SetTextureType(TextureType::PrefilterMap);
    m_PrefilterTexture->SetUniformLocation("material.prefilterMap");
    m_PrefilterTexture->GenerateMipMap();
    
    // pbr: run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
    m_PrefilterShader->Bind();
    m_PrefilterShader->SetUniform1i("environmentMap", 0);
    m_PrefilterShader->SetUniformMat4f("projection", m_CaptureProjection);

	m_ReflectionTexture->Bind(0);

	constexpr unsigned int maxMipLevels = 5;
    for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
    {
        // resize framebuffer according to mip-level size.
        const auto mipWidth = static_cast<unsigned int>(128 * std::pow(0.5, mip));
        const auto mipHeight = static_cast<unsigned int>(128 * std::pow(0.5, mip));
        m_RBO.CreateRenderBufferStorage(mipWidth, mipHeight, FramebufferTextureFormat::Depth24);
        glViewport(0, 0, mipWidth, mipHeight);

        float roughness = (float)mip / (float)(maxMipLevels - 1);
        m_PrefilterShader->SetUniform1f("roughness", roughness);
        for (unsigned int i = 0; i < 6; ++i)
        {
            m_PrefilterShader->SetUniformMat4f("view", m_CaptureViews[i]);
            m_FBO.AttachColorTexture3D(i, m_PrefilterTexture, mip);
            Renderer::RenderCube();
        }
    }
    
    m_PrefilterShader->Unbind();

}

void ReflectionProbe::CreateBRDFLookUpTexture()
{
    m_BrdfLookUpTexture->SetTexture2DSize(512, 512);
    m_BrdfLookUpTexture->CreateTexture2DStorage(TextureInternalFormat::Rg16F);
    m_BrdfLookUpTexture->SetWrapMode(TextureWrap::ClampToEdge, TextureWrap::ClampToEdge);
    m_BrdfLookUpTexture->SetFilter(TextureFilter::Linear, TextureFilter::Linear);
    m_BrdfLookUpTexture->SetTextureType(TextureType::BrdfLookUpTexture);
    m_BrdfLookUpTexture->SetUniformLocation("material.brdfLUT");
    
    m_RBO.CreateRenderBufferStorage(512, 512, FramebufferTextureFormat::Depth24);
	m_FBO.AttachColorTexture2D(m_BrdfLookUpTexture);

    m_BrdfShader->Bind();
    
    glViewport(0, 0, 512, 512);
	
    Renderer::RenderQuad();
    m_BrdfShader->Unbind();
}