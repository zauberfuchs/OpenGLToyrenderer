#include "Engine/Utils/pch.h"
#include "SkyboxPass.h"

#include "Engine/Renderer/Renderer.h"

SkyboxPass::SkyboxPass()
{
}

void SkyboxPass::Init()
{
	TextureDesc desc = {};
	desc.Target = TextureTarget::Texture2D;
	desc.Format = TextureInternalFormat::R8;
	desc.Width = 8;
	desc.Height = 8;
	desc.MipLevels = 1;
	desc.SampleCount = 1;
	desc.DebugName = "DitherTexture";
	
	m_DitherTexture = CreateRef<Texture>(desc);
	
	m_DitherTexture->SetWrapMode(TextureWrap::Repeat, TextureWrap::Repeat);
	m_DitherTexture->SetFilter(TextureFilter::Nearest, TextureFilter::Nearest);
	
	m_DitherTexture->SetTexture2DByteData(m_BayerMatrix);
}

void SkyboxPass::Execute(ForwardRenderContext& rendererContext)
{
	
	Renderer::BeginEvent("Skybox Pass");
	
	rendererContext.ForwardFrameBuffer->Bind();
	if (rendererContext.SceneSkybox != nullptr)
	{
		m_SkyboxShader = World::Get().GetShader("skybox");
		m_SkyboxShader->Bind();
		m_DitherTexture->Bind(1);
		m_SkyboxShader->SetUniformMat4f("view", glm::mat4(glm::mat3(rendererContext.ActiveSceneCamera->GetViewMatrix())));// this removes the translation from the view matrix
		m_SkyboxShader->SetUniformMat4f("projection", rendererContext.ActiveSceneCamera->GetProjectionMatrix());
		rendererContext.SceneSkybox->Bind();
		Renderer::RenderSkyBox();
		rendererContext.SceneSkybox->Unbind();
		m_DitherTexture->Unbind();
	}
	rendererContext.ForwardFrameBuffer->Unbind();
	
	Renderer::EndEvent();
}


