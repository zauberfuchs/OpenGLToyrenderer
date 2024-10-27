#include "Engine/Utils/pch.h"
#include "ShadowPass.h"

#include "Engine/Components/Model.h"
#include "Engine/Renderer/Renderer.h"


ShadowPass::ShadowPass()
{
}

void ShadowPass::Init()
{
}

void ShadowPass::Execute(ForwardRenderContext& rendererContext)
{
	// safes the current viewport size.
	int ViewportWidth = rendererContext.ActiveSceneCamera->Viewport.Width;
	uint ViewportHeight = rendererContext.ActiveSceneCamera->Viewport.Height;

	rendererContext.ActiveLights.begin()->second->GetFramebuffer()->Bind();

	switch(rendererContext.ActiveLights.begin()->second->GetType()) {
		case LightSourceType::DirectionalLight:
			m_ShadowShader = World::Get().GetShader("dirLightDepthMap");
			m_ShadowShader->Bind();
			break;
		case LightSourceType::SpotLight:
			m_ShadowShader = World::Get().GetShader("spotLightDepthMap");
			m_ShadowShader->Bind();
			break;
		case LightSourceType::PointLight:
			m_ShadowShader = World::Get().GetShader("pointLightDepthMap");
			m_ShadowShader->Bind();
			break;
	}

	//set the Viewport size to shadow texture size
	glViewport(0, 0, rendererContext.ActiveLights.begin()->second->GetShadowWidth(), rendererContext.ActiveLights.begin()->second->GetShadowHeight());
	
	
	glClear(GL_DEPTH_BUFFER_BIT);
	EntityID eID = rendererContext.ActiveLights.begin()->first;
	Transform& t = World::Get().GetEntityManager()->GetComponent<Transform>(eID);
	auto shadowMatrices = rendererContext.ActiveLights.begin()->second->CreateShadowTransformMatrices(0.1f, 50.0f, t.GetLocalPosition());
	for (unsigned int i = 0; i < 6; ++i)
	{
		m_ShadowShader->SetUniformMat4f("shadowMatrices[" + std::to_string(i) + "]", shadowMatrices[i]);
	}
	
	m_ShadowShader->SetUniform1f("farPlane", rendererContext.ActiveSceneCamera->GetFarPlane());
	
	m_ShadowShader->SetUniform3f("lightPos", t.GetLocalPosition());
	
	
	for (const auto& [name, SceneObj] : rendererContext.ActiveScene->GetSceneObjects())
	{
		if (SceneObj->HasComponent<Model>()) 
		{
			m_ShadowShader->SetUniformMat4f("model", SceneObj->GetComponent<Transform>().GetTransformMatrix());
			for (const Ref<Mesh>& mesh : SceneObj->GetComponent<Model>().GetMeshes())
			{
				Renderer::DrawMesh(mesh, true);
			}
		}
		
	}
	rendererContext.ActiveLights.begin()->second->GetFramebuffer()->Unbind();

	// restore old Viewport size
	Renderer::SetViewport(0, 0, ViewportWidth, ViewportHeight);
}