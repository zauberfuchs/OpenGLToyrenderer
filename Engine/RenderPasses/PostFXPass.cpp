#include "Engine/Utils/pch.h"
#include "PostFXPass.h"

PostFXPass::PostFXPass()
{
}

void PostFXPass::Init()
{
	m_PostFXShader = World::Get().GetShader("postFX");
}

void PostFXPass::Execute(RendererContext& rendererContext)
{
	m_PostFXShader->Bind();
	m_PostFXShader->SetUniform1i("sampleSize", rendererContext.MSAA);
	m_PostFXShader->SetUniform1i("screenTexture", 0);
	
	m_ViewportTexture->Bind();
	RenderQuad();
	m_ViewportTexture->Unbind();
}

void PostFXPass::RenderQuad()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	uint vao;
	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}