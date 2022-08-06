#include "../../Engine/Utils/pch.h"
#include "Renderer.h"


void Renderer::Init()
{
	UpdateViewport();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	
	s_Data.MSAA = new int(1);

	s_Data.GeometryFramebuffer = new Framebuffer();
	s_Data.GeometryFramebuffer->SetSampleSize(*s_Data.MSAA);
	s_Data.GeometryRenderbuffer = new Renderbuffer();
	s_Data.GeometryRenderbuffer->SetSampleSize(*s_Data.MSAA);



	s_Data.ActiveScene = World::Get().GetActiveScene();


	if (s_Data.ActiveScene->GetSceneSkybox())
	{
		s_Data.SceneSkybox = s_Data.ActiveScene->GetSceneSkybox();
	}
	s_Data.PostFXShader = World::Get().GetShader("postFX");
	s_Data.ActiveSceneLight = World::Get().GetActiveScene()->GetSceneLightSources().begin()->second;

	
}

void Renderer::Shutdown()
{
	delete s_Data.GeometryFramebuffer;
}

void Renderer::DepthPrePath()
{
	//Todo Update Rendererdata Function?
	UpdateViewport();

	s_Data.GeometryFramebuffer->SetFramebufferTextureSize(s_Data.RenderViewport[2], s_Data.RenderViewport[3]);
	s_Data.GeometryFramebuffer->CreateColorTexture(true);
	s_Data.GeometryRenderbuffer->CreateRenderBufferStorage(s_Data.RenderViewport[2], s_Data.RenderViewport[3], FramebufferTextureFormat::Depth32Stencil8);
	s_Data.GeometryFramebuffer->AttachRenderBuffer(s_Data.GeometryRenderbuffer->GetId(), FramebufferAttachment::DepthStencil);


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
	auto shadowMatrices = s_Data.ActiveSceneLight->CreateShadowTransformMatrices(0.1f, 50.0f);
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
	s_Data.GeometryFramebuffer->Bind();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
			s_Data.MeshMaterial->SetupUniforms();

			DrawMesh();

			s_Data.MeshMaterial->UnbindTextures();
		}
	}
}

void Renderer::SkyboxPath()
{
	s_Data.SceneSkybox->Render();
}

void Renderer::PostFxPath()
{
	s_Data.GeometryFramebuffer->Unbind();
	s_Data.PostFXShader->Bind();
	s_Data.PostFXShader->SetUniform1i("sampleSize", *s_Data.MSAA);
	s_Data.PostFXShader->SetUniform1i("screenTexture", 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, s_Data.GeometryFramebuffer->GetColorTextureId());
	RenderQuad();
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
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

unsigned int quadVAO = 0;
unsigned int quadVBO;
void Renderer::RenderQuad()
{
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
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}