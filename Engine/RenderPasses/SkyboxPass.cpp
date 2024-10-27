#include "Engine/Utils/pch.h"
#include "SkyboxPass.h"

#include "Engine/Renderer/Renderer.h"

SkyboxPass::SkyboxPass()
{
}

void SkyboxPass::Init()
{
}

void SkyboxPass::Execute(ForwardRenderContext& rendererContext)
{
	rendererContext.ForwardFrameBuffer->Bind();
	if (rendererContext.SceneSkybox != nullptr)
	{
		m_SkyboxShader = World::Get().GetShader("skybox");
		m_SkyboxShader->Bind();
		m_SkyboxShader->SetUniformMat4f("view", glm::mat4(glm::mat3(rendererContext.ActiveSceneCamera->GetViewMatrix())));// this removes the translation from the view matrix
		m_SkyboxShader->SetUniformMat4f("projection", rendererContext.ActiveSceneCamera->GetProjectionMatrix());
		rendererContext.SceneSkybox->Bind();
		Renderer::RenderSkyBox();
		rendererContext.SceneSkybox->Unbind();
	}
	rendererContext.ForwardFrameBuffer->Unbind();
}