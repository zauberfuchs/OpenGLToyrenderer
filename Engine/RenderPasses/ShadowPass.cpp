#include "Engine/Utils/pch.h"
#include "ShadowPass.h"
#include "Engine/Components/Model.h"

ShadowPass::ShadowPass()
{
}

void ShadowPass::Init()
{

}

void ShadowPass::Execute(RendererContext& rendererContext)
{
	// safes the current viewport size.
	int ViewportWidth = rendererContext.ActiveSceneCamera->Viewport.Width;
	uint ViewportHeight = rendererContext.ActiveSceneCamera->Viewport.Height;

	m_Lights.begin()->second->GetFramebuffer()->Bind();

	switch(m_Lights.begin()->second->GetType()) {
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
	//Renderer::SetViewport(0, 0, m_Lights.begin()->second->GetShadowWidth(), m_Lights.begin()->second->GetShadowHeight());

	glClear(GL_DEPTH_BUFFER_BIT);
	EntityID eID = m_Lights.begin()->first;
	Transform& t = World::Get().GetEntityManager()->GetComponent<Transform>(eID);
	auto shadowMatrices = m_Lights.begin()->second->CreateShadowTransformMatrices(0.1f, 50.0f, t.GetLocalPosition());
	for (unsigned int i = 0; i < 6; ++i)
	{
		m_ShadowShader->SetUniformMat4f("shadowMatrices[" + std::to_string(i) + "]", shadowMatrices[i]);
	}
	
	m_ShadowShader->SetUniform1f("farPlane", m_Lights.begin()->second->GetFarPlane());
	
	m_ShadowShader->SetUniform3f("lightPos", t.GetLocalPosition());
	
	
	for (const auto& [name, SceneObj] : rendererContext.ActiveScene->GetSceneObjects())
	{
		rendererContext.MeshTransform = SceneObj->GetComponent<Transform>(); 
		if (SceneObj->HasComponent<Model>()) 
		{
			for (const Ref<Mesh>& m : SceneObj->GetComponent<Model>().GetMeshes())
			{
				rendererContext.MeshVAO = m->GetVAO();
				rendererContext.MeshIndexCount = m->GetIndexCount();
				rendererContext.MeshRenderMode = m->GetRenderMode();
				rendererContext.MeshMaterial = nullptr;

				rendererContext.MeshVAO->Bind();

				//DrawMesh();
			}
		}
		
	}
	m_Lights.begin()->second->GetFramebuffer()->Unbind();

	// restore old Viewport size
	//Renderer::SetViewport(0, 0, ViewportWidth, ViewportHeight);
}