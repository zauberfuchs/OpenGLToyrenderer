#include "Engine/Utils/pch.h"
#include "ReflectionProbe.h"

#include "Renderer.h"
#include "World.h"


ReflectionProbe::ReflectionProbe(const int& width, const int& height)
	: m_BrdfLookUpTexture(TextureTarget::Texture2D),
		m_IrradianceTexture(TextureTarget::TextureCubeMap),
		m_PrefilterTexture(TextureTarget::TextureCubeMap),
		m_ReflectionTexture(TextureTarget::TextureCubeMap),
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
	CreateIrradianceMap();
	CreatePrefilterMap();
	CreateBRDFLookUpTexture();
}

void ReflectionProbe::CreateReflectionMapFromHDR(const std::string& path)
{
    m_FBO.Bind();
    m_RBO.Bind();
    m_FBO.AttachRenderBuffer(m_RBO.GetId(), FramebufferAttachment::Depth);
    m_RBO.CreateRenderBufferStorage(1024, 1024, FramebufferTextureFormat::Depth24);

    // load the HDR environment map
    //todo Texture erstellung überarbeiten, was soll der konstruktor bekommen und was die create methode.
    Texture envHDRMap(TextureTarget::Texture2D);
    envHDRMap.Load(path, TextureWrap::ClampToEdge, TextureFilter::Linear);
    envHDRMap.Bind(0);

    // setup cubemap to render to and attach to framebuffer
    m_ReflectionTexture.SetTexture2DSize(512, 512);
    m_ReflectionTexture.SetWrapMode(TextureWrap::ClampToEdge, TextureWrap::ClampToEdge, TextureWrap::ClampToEdge);
    m_ReflectionTexture.SetFilter(TextureFilter::Linear, TextureFilter::Linear);
    m_ReflectionTexture.CreateTextureCubeMapStorage(TextureInternalFormat::Rgb16F);

    // convert HDR equirectangular environment map to cubemap equivalent
    m_EquirectangularToCubemapShader->Bind();
    m_EquirectangularToCubemapShader->SetUniform1i("equirectangularMap", 0);
    m_EquirectangularToCubemapShader->SetUniformMat4f("projection", m_CaptureProjection);

    glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
    for (unsigned int i = 0; i < 6; ++i)
    {
        m_EquirectangularToCubemapShader->SetUniformMat4f("view", m_CaptureViews[i]);
        m_FBO.AttachColorTexture3D(i, m_ReflectionTexture);
        //Renderer::RenderCube();
        RenderCube();
    }
    
    m_FBO.Unbind();
    m_EquirectangularToCubemapShader->Unbind();
}

void ReflectionProbe::SetReflectionMap(const Texture& texture)
{
    m_ReflectionTexture = texture;
}

void ReflectionProbe::CreateIrradianceMap()
{
    m_IrradianceTexture.SetTexture2DSize(32, 32);
    m_IrradianceTexture.SetWrapMode(TextureWrap::ClampToEdge, TextureWrap::ClampToEdge, TextureWrap::ClampToEdge);
    m_IrradianceTexture.SetFilter(TextureFilter::Linear, TextureFilter::Linear);
    m_IrradianceTexture.SetTextureType(TextureType::IrradianceMap);
    m_IrradianceTexture.SetUniformLocation("material.irradianceMap");
    m_IrradianceTexture.CreateTextureCubeMapStorage(TextureInternalFormat::Rgb16F);


	m_FBO.AttachRenderBuffer(m_RBO.GetId(), FramebufferAttachment::Depth);
    m_RBO.CreateRenderBufferStorage(32, 32, FramebufferTextureFormat::Depth24);


    // pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
    m_IrradianceShader->Bind();
    m_IrradianceShader->SetUniform1i("environmentMap", 0);
    m_IrradianceShader->SetUniformMat4f("projection", m_CaptureProjection);

    m_ReflectionTexture.Bind(0);
    
    glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
    for (unsigned int i = 0; i < 6; ++i)
    {
        m_IrradianceShader->SetUniformMat4f("view", m_CaptureViews[i]);
        m_FBO.AttachColorTexture3D(i, m_IrradianceTexture);
        //Renderer::RenderCube();
		RenderCube();
    }

    m_FBO.Unbind();
    m_IrradianceShader->Unbind();
}


void ReflectionProbe::CreatePrefilterMap()
{
    m_PrefilterTexture.SetTexture2DSize(128, 128);
    m_PrefilterTexture.SetWrapMode(TextureWrap::ClampToEdge, TextureWrap::ClampToEdge, TextureWrap::ClampToEdge);
    m_PrefilterTexture.SetFilter(TextureFilter::MipMapLinear, TextureFilter::Linear);
    m_PrefilterTexture.SetTextureType(TextureType::PrefilterMap);
    m_PrefilterTexture.SetUniformLocation("material.prefilterMap");
    m_PrefilterTexture.CreateTextureCubeMapStorage(TextureInternalFormat::Rgb16F, true);
    m_PrefilterTexture.GenerateMipMap();
    
    // pbr: run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
    m_PrefilterShader->Bind();
    m_PrefilterShader->SetUniform1i("environmentMap", 0);
    m_PrefilterShader->SetUniformMat4f("projection", m_CaptureProjection);

	m_ReflectionTexture.Bind(0);

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
            //Renderer::RenderCube();
        	RenderCube();
        }
    }

    m_FBO.Unbind();
    m_PrefilterShader->Unbind();

}

void ReflectionProbe::CreateBRDFLookUpTexture()
{
    m_BrdfLookUpTexture.SetWrapMode(TextureWrap::ClampToEdge, TextureWrap::ClampToEdge);
    m_BrdfLookUpTexture.SetFilter(TextureFilter::Linear, TextureFilter::Linear);
    m_BrdfLookUpTexture.SetTexture2DSize(512, 512);
    m_BrdfLookUpTexture.CreateTexture2DStorage(TextureInternalFormat::Rg16F);
    m_BrdfLookUpTexture.SetTextureType(TextureType::BrdfLookUpTexture);
    m_BrdfLookUpTexture.SetUniformLocation("material.brdfLUT");
    
    m_RBO.CreateRenderBufferStorage(512, 512, FramebufferTextureFormat::Depth24);
	m_FBO.AttachColorTexture2D(m_BrdfLookUpTexture);

    m_BrdfShader->Bind();
    
    glViewport(0, 0, 512, 512);
    Renderer::RenderQuad();

    m_FBO.Unbind();
    m_BrdfShader->Unbind();
}

void ReflectionProbe::RenderCube()
{
	float vertices[] = {
	    // back face
	    -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
	     1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
	     1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
	     1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
	    -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
	    -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
	    // front face
	    -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
	     1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
	     1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
	     1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
	    -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
	    -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
	    // left face
	    -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
	    -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
	    -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
	    -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
	    -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
	    -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
	    // right face
	     1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
	     1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
	     1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
	     1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
	     1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
	     1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
	    // bottom face
	    -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
	     1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
	     1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
	     1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
	    -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
	    -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
	    // top face
	    -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
	     1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
	     1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
	     1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
	    -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
	    -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
	};

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glCreateBuffers(1, &m_CubeVBO);
    glNamedBufferData(m_CubeVBO, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glCreateVertexArrays(1, &m_CubeVAO);

    glVertexArrayVertexBuffer(m_CubeVAO, 0, m_CubeVBO, 0, 8 * sizeof(float));

    glEnableVertexArrayAttrib(m_CubeVAO, 0);
    glEnableVertexArrayAttrib(m_CubeVAO, 1);

    glVertexArrayAttribFormat(m_CubeVAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(m_CubeVAO, 1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
    glVertexArrayAttribFormat(m_CubeVAO, 2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float));

    glVertexArrayAttribBinding(m_CubeVAO, 0, 0);
    glVertexArrayAttribBinding(m_CubeVAO, 1, 0);
    glVertexArrayAttribBinding(m_CubeVAO, 2, 0);

    glBindVertexArray(m_CubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

}
