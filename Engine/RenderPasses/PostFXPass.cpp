#include "Engine/Utils/pch.h"
#include "PostFXPass.h"

#include "Engine/Renderer/Renderer.h"

PostFXPass::PostFXPass()
{
}

void PostFXPass::Init()
{
	m_PostFXShader = World::Get().GetShader("postFX");
}

void PostFXPass::Execute(ForwardRenderContext& rendererContext)
{
	Renderer::BeginEvent("Postprocessing Pass");
	
	m_PostFXShader->Bind();
	m_PostFXShader->SetUniform1i("sampleSize", rendererContext.MSAA);
	m_PostFXShader->SetUniform1i("screenTexture", 0);
	
	rendererContext.ViewportTexture->Bind();
	Renderer::RenderQuad();
	rendererContext.ViewportTexture->Unbind();
	
	Renderer::EndEvent();
}