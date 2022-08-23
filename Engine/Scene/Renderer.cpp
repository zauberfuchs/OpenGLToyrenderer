#include "Engine/Utils/pch.h"
#include "Renderer.h"

#include "Engine/Components/Model.h"

void Renderer::Init()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	
	s_Data.MSAA = new int(1);

	s_Data.GeometryFramebuffer = new Framebuffer();
	s_Data.GeometryRenderbuffer = new Renderbuffer();
	s_Data.ViewportTexture = new Texture(TextureTarget::Texture2DMultiSample);
	s_Data.ViewportTexture->SetFilter(TextureFilter::Nearest, TextureFilter::Nearest);


	s_Data.ActiveScene = World::Get().GetActiveScene();
	s_Data.ActiveSceneCamera = s_Data.ActiveScene->GetSceneCamera();

	for(auto m : World::Get().GetMaterials())
	{
		m.second->SetReflectionProbe(s_Data.ActiveScene->GetReflectionProbe());
	}

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

	s_Data.GeometryFramebuffer->SetSampleSize(*s_Data.MSAA);
	s_Data.GeometryRenderbuffer->SetSampleSize(*s_Data.MSAA);
	s_Data.GeometryFramebuffer->SetFramebufferTextureSize(s_Data.RenderViewport[2], s_Data.RenderViewport[3]);
	
	s_Data.ViewportTexture->SetTexture2DSize(s_Data.RenderViewport[2], s_Data.RenderViewport[3]);
	s_Data.ViewportTexture->CreateTexture2DStorage(TextureInternalFormat::Rgb16, false, *s_Data.MSAA);
	s_Data.GeometryFramebuffer->AttachColorTexture2D(*s_Data.ViewportTexture);

	
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
	s_Data.ActiveSceneLight->GetFramebuffer()->Bind();

	glClear(GL_DEPTH_BUFFER_BIT);

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
	s_Data.ActiveSceneLight->GetFramebuffer()->Unbind();

	SetViewport(s_Data.RenderViewport[0], s_Data.RenderViewport[1], s_Data.RenderViewport[2], s_Data.RenderViewport[3]);
}

void Renderer::GeometryPath()
{
	s_Data.GeometryFramebuffer->Bind();

	Clear();

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
			s_Data.ActiveSceneCamera->UpdateMatrix(s_Data.ActiveShader);
			s_Data.ActiveShader->SetUniform3f("camPos", s_Data.ActiveSceneCamera->Position);


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
	if (s_Data.SceneSkybox != nullptr)
	{
		s_Data.ActiveShader = World::Get().GetShader("skybox");
		s_Data.ActiveShader->Bind();
		s_Data.ActiveShader->SetUniformMat4f("view", glm::mat4(glm::mat3(s_Data.ActiveSceneCamera->GetViewMatrix())));// this removes the translation from the view matrix
		s_Data.ActiveShader->SetUniformMat4f("projection", s_Data.ActiveSceneCamera->GetProjectionMatrix());
		s_Data.SceneSkybox->Bind(0);
		RenderCube();
	}
	s_Data.GeometryFramebuffer->Unbind();
}

void Renderer::PostFxPath()
{
	s_Data.PostFXShader->Bind();
	s_Data.PostFXShader->SetUniform1i("sampleSize", *s_Data.MSAA);
	s_Data.PostFXShader->SetUniform1i("screenTexture", 0);
	
	s_Data.ViewportTexture->Bind(0);
	RenderQuad();
	s_Data.ViewportTexture->Unbind();
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
unsigned int quadVBO = 0;
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


unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
unsigned int cubeEBO = 0;
void Renderer::RenderCube()
{
	if (cubeVAO == 0)
	{
		float skyboxVertices[24] =
		{
			//   Coordinates
			-1.0f, -1.0f,  1.0f,//        7--------6
			 1.0f, -1.0f,  1.0f,//       /|       /|
			 1.0f, -1.0f, -1.0f,//      4--------5 |
			-1.0f, -1.0f, -1.0f,//      | |      | |
			-1.0f,  1.0f,  1.0f,//      | 3------|-2
			 1.0f,  1.0f,  1.0f,//      |/       |/
			 1.0f,  1.0f, -1.0f,//      0--------1
			-1.0f,  1.0f, -1.0f
		};

		unsigned int skyboxIndices[] =
		{
			// Right
			6, 2, 1,
			1, 5, 6,
			// Left
			7, 4, 0,
			0, 3, 7,
			// Top
			6, 5, 4,
			4, 7, 6,
			// Bottom
			2, 3, 0,
			0, 1, 2,
			// Back
			5, 1, 0,
			0, 4, 5,
			// Front
			6, 7, 3,
			3, 2, 6
		};

		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		glGenBuffers(1, &cubeEBO);
		glBindVertexArray(cubeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	}

	glBindVertexArray(cubeVAO);
	glDepthFunc(GL_LEQUAL);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDepthFunc(GL_LESS);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
