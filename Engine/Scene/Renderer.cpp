#include "../../Engine/Utils/pch.h"
#include "Renderer.h"

#include "../../Engine/Scene/World.h"

struct RendererStorage
{
	Scene* ActiveScene;
	IShader* ActiveShader;

	VertexArray* MeshVAO;

	uint16_t MeshIndexCount;
	uint16_t MeshRenderMode;

	IMaterial* MeshMaterial;
	Transform* MeshTransform;

};

static RendererStorage s_Data;

void Renderer::Init()
{
	s_Data.ActiveScene = World::Get().GetActiveScene();
}

void Renderer::Shutdown()
{

}

void Renderer::ShadowPrepath()
{
	//Todo enum class von shadertypen?
	s_Data.ActiveShader = World::Get().GetShader("shadowmapShader");

	Light* l = World::Get().GetActiveScene()->GetSceneLightSources().begin()->second;

	glViewport(0, 0, l->GetShadowWidth(), l->GetShadowHeight());
	glBindFramebuffer(GL_FRAMEBUFFER, l->GetFramebuffer().GetId());
	glClear(GL_DEPTH_BUFFER_BIT);

	for (const auto& s : s_Data.ActiveScene->GetSceneObjects())
	{
		s_Data.MeshTransform = s.second->GetTransform();
		for (const auto& m : s.second->GetModel().GetMeshes())
		{
			s_Data.MeshVAO = m.second->GetVAO();
			s_Data.MeshIndexCount = m.second->GetIndicesSize();
			s_Data.MeshRenderMode = m.second->GetRenderMode();
			s_Data.MeshMaterial = nullptr;

			DrawMesh();
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//RenderScreenQuad();
}

void Renderer::GeometryPath()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, 1280, 1024);
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

			DrawMesh();
		}
	}
}


void Renderer::DrawMesh()
{
	//Todo light wirkt hier etwas fehl am platz
	Light* l = World::Get().GetActiveScene()->GetSceneLightSources().begin()->second;
	s_Data.MeshVAO->Bind();
	s_Data.ActiveShader->Bind();
	s_Data.ActiveShader->SetUniformMat4f("model", s_Data.MeshTransform->GetTransformMatrix());
	s_Data.ActiveShader->SetUniformMat4f("lightSpaceMatrix", l->CreateLightSpaceMatrix());
	if (s_Data.MeshMaterial)
	{
		//Todo wrong place??
		s_Data.MeshMaterial->SetTexture(l->GetDepthmap());
		s_Data.MeshMaterial->Render();
	}

	glDrawElements(s_Data.MeshRenderMode, s_Data.MeshIndexCount, GL_UNSIGNED_INT, nullptr);

	if (s_Data.MeshMaterial)
	{
		s_Data.MeshMaterial->RenderPost();
	}
}


unsigned int quadVAO = 0;
unsigned int quadVBO;
void Renderer::RenderScreenQuad()
{
	Light* l = World::Get().GetActiveScene()->GetSceneLightSources().begin()->second;
	

	s_Data.ActiveShader = World::Get().GetShader("debugQuad");
	s_Data.ActiveShader->Bind();
	s_Data.ActiveShader->SetUniform1i("depthMap", 0);
	s_Data.ActiveShader->SetUniform1f("near_plane", 1.0f);
	s_Data.ActiveShader->SetUniform1f("far_plane", 7.5f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, l->GetDepthmap()->m_ID);

	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}