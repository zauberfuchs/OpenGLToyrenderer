#include "../../Engine/Utils/pch.h"
#include "Renderer.h"

#include "Skybox.h"
#include "../../Engine/Scene/World.h"

struct RendererStorage
{
	Scene* ActiveScene;
	IShader* ActiveShader;

	Skybox* SceneSkybox;

	VertexArray* MeshVAO;

	uint32_t MeshIndexCount;
	uint16_t MeshRenderMode;

	IMaterial* MeshMaterial;
	Transform* MeshTransform;

	Light* ActiveSceneLight;

	GLint RenderViewport[4];

};

static RendererStorage s_Data;

void Renderer::Init()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	s_Data.ActiveScene = World::Get().GetActiveScene();
	if (s_Data.ActiveScene->GetSceneSkybox())
	{
		s_Data.SceneSkybox = s_Data.ActiveScene->GetSceneSkybox();
	}

	s_Data.ActiveSceneLight = World::Get().GetActiveScene()->GetSceneLightSources().begin()->second;
}

void Renderer::Shutdown()
{

}

void Renderer::SkyboxPath()
{
	s_Data.SceneSkybox->Render();
}

void Renderer::DepthPrePath()
{
	//Todo Update Rendererdata Function?
	UpdateViewport();
	
	
	switch(s_Data.ActiveSceneLight->GetType()) {
	case LightSourceType::DirectionalLight:
		s_Data.ActiveShader = World::Get().GetShader("dirLightDepthMap");
		s_Data.ActiveShader->Bind();
		break;
	case LightSourceType::SpotLight:
		s_Data.ActiveShader = World::Get().GetShader("spotLightDepthMap");
		s_Data.ActiveShader->Bind();
		break;
	case LightSourceType::PointLight:
		s_Data.ActiveShader = World::Get().GetShader("pointLightDepthMap");
		s_Data.ActiveShader->Bind();
		break;
	}
	
	

	glViewport(0, 0, s_Data.ActiveSceneLight->GetShadowWidth(), s_Data.ActiveSceneLight->GetShadowHeight());
	glBindFramebuffer(GL_FRAMEBUFFER, s_Data.ActiveSceneLight->GetFramebuffer()->GetId());
	glClear(GL_DEPTH_BUFFER_BIT);

	
	//todo maybe let the light do it
	auto shadowMatrices = s_Data.ActiveSceneLight->CreateShadowTransformMatrices(1.0f, 25.0f);
	for (unsigned int i = 0; i < 6; ++i)
	{
		s_Data.ActiveShader->SetUniformMat4f("shadowMatrices[" + std::to_string(i) + "]", shadowMatrices[i]);
	}
	s_Data.ActiveShader->SetUniform1f("farPlane", s_Data.ActiveSceneLight->GetFarPlane());
	s_Data.ActiveShader->SetUniform3f("lightPos", s_Data.ActiveSceneLight->GetPosition());


	for (const auto& s : s_Data.ActiveScene->GetSceneObjects())
	{
		s_Data.MeshTransform = s.second->GetTransform(); 
		for (const auto& m : s.second->GetModel().GetMeshes())
		{
			s_Data.MeshVAO = m.second->GetVAO();
			s_Data.MeshIndexCount = m.second->GetIndicesSize();
			s_Data.MeshRenderMode = m.second->GetRenderMode();
			s_Data.MeshMaterial = nullptr;
			
			s_Data.MeshVAO->Bind();

			DrawMesh();
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	SetViewport(s_Data.RenderViewport[0], s_Data.RenderViewport[1], s_Data.RenderViewport[2], s_Data.RenderViewport[3]);
}

void Renderer::GeometryPath()
{
	//get windowsize?
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glViewport(0, 0, 1280, 1024);

	for (const auto& s : s_Data.ActiveScene->GetSceneObjects())
	{
		s_Data.MeshTransform = s.second->GetTransform();
		for (const auto& m : s.second->GetModel().GetMeshes())
		{
			s_Data.ActiveShader = m.second->GetMaterial()->GetShader();
			s_Data.MeshVAO = m.second->GetVAO();
			s_Data.MeshIndexCount = m.second->GetIndicesSize();
			s_Data.MeshRenderMode = m.second->GetRenderMode();
			s_Data.MeshMaterial = m.second->GetMaterial();

			s_Data.ActiveShader->Bind();
			s_Data.MeshVAO->Bind();

			s_Data.ActiveShader->SetUniform1f("farPlane", s_Data.ActiveSceneLight->GetFarPlane());
			s_Data.MeshMaterial->SetTexture(s_Data.ActiveSceneLight->GetDepthmap());
			s_Data.MeshMaterial->Render();

			DrawMesh();

			s_Data.MeshMaterial->RenderPost();
		}
	}
}


void Renderer::DrawMesh()
{
	s_Data.ActiveShader->SetUniformMat4f("model", s_Data.MeshTransform->GetTransformMatrix());
	
	glDrawElements(s_Data.MeshRenderMode, s_Data.MeshIndexCount, GL_UNSIGNED_INT, nullptr);
}



void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	glViewport(x, y, width, height);
}

void Renderer::UpdateViewport()
{
	glGetIntegerv(GL_VIEWPORT, s_Data.RenderViewport);
}

void Renderer::SetClearColor(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::SetCullMode(const uint8_t& cullMode)
{
	glCullFace(cullMode);
}
