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
	StoreViewportSize();

	s_Data.MSAA = new int(1);
	s_Data.GeometryFramebuffer = new Framebuffer();
	s_Data.GeometryRenderbuffer = new Renderbuffer();
	s_Data.GeometryFramebuffer->AttachRenderBuffer(s_Data.GeometryRenderbuffer->GetId(), FramebufferAttachment::DepthStencil);

	
	s_Data.ViewportTexture = new Texture(TextureTarget::Texture2DMultiSample);
	s_Data.ViewportTexture->SetTexture2DSize(s_Data.RenderViewport[2], s_Data.RenderViewport[3]);
	s_Data.ViewportTexture->CreateTexture2DStorage(TextureInternalFormat::Rgb16, false, *s_Data.MSAA);

	//todo wirft nen fehler
	s_Data.ViewportTexture->SetFilter(TextureFilter::Nearest, TextureFilter::Nearest);
	std::cout << glGetError() << std::endl;
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
	// safes the current viewport size.
	StoreViewportSize();

	s_Data.ActiveSceneLight->GetFramebuffer()->Bind();

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

	//set the Viewport size to shadow texture size
	SetViewport(0, 0, s_Data.ActiveSceneLight->GetShadowWidth(), s_Data.ActiveSceneLight->GetShadowHeight());

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

	// restore old Viewport size
	SetViewport(s_Data.RenderViewport[0], s_Data.RenderViewport[1], s_Data.RenderViewport[2], s_Data.RenderViewport[3]);
}

void Renderer::GeometryPath()
{
	s_Data.GeometryFramebuffer->Bind();
	
	

	// wenn der viewport sich verändert oder die samplesize
	if (s_Data.ViewportTexture->GetWidth() != s_Data.RenderViewport[2] || s_Data.ViewportTexture->GetHeight() != s_Data.RenderViewport[3] || s_Data.GeometryFramebuffer->GetSampleSize() != *s_Data.MSAA)
	{
		// setzt die anzahl der Samples pro Pixel
		s_Data.GeometryFramebuffer->SetSampleSize(*s_Data.MSAA);
		s_Data.GeometryRenderbuffer->SetSampleSize(*s_Data.MSAA);

		// fügt die textur als color attachment dem framebuffer hintu
		s_Data.ViewportTexture->SetTexture2DSize(s_Data.RenderViewport[2], s_Data.RenderViewport[3]);
		s_Data.ViewportTexture->CreateTexture2DStorage(TextureInternalFormat::Rgb16, false, *s_Data.MSAA);
		s_Data.GeometryFramebuffer->AttachColorTexture2D(*s_Data.ViewportTexture);
		s_Data.ViewportTexture->SetFilter(TextureFilter::Nearest, TextureFilter::Nearest);

		// fügt den Renderbuffer als Depth/stencil attachment dem Framebuffer hinzu da wir die depth informationen brauchen
		s_Data.GeometryRenderbuffer->CreateRenderBufferStorage(s_Data.RenderViewport[2], s_Data.RenderViewport[3], FramebufferTextureFormat::Depth32Stencil8);
	}

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
		RenderSkyBox();
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

void Renderer::StoreViewportSize()
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (quadVAO == 0)
	{
		float vertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glCreateBuffers(1, &quadVBO);
		glNamedBufferData(quadVBO, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glCreateVertexArrays(1, &quadVAO);

		glVertexArrayVertexBuffer(quadVAO, 0, quadVBO, 0, 5 * sizeof(float));

		glEnableVertexArrayAttrib(quadVAO, 0);
		glEnableVertexArrayAttrib(quadVAO, 1);

		glVertexArrayAttribFormat(quadVAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribFormat(quadVAO, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float));

		glVertexArrayAttribBinding(quadVAO, 0, 0);
		glVertexArrayAttribBinding(quadVAO, 1, 0);
	}
	glBindVertexArray(quadVAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

unsigned int skyBoxVAO = 0;
unsigned int skyBoxVBO = 0;
unsigned int skyBoxEBO = 0;
void Renderer::RenderSkyBox()
{
	if (skyBoxVAO == 0)
	{
		float vertices[24] =
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

		unsigned int indices[] =
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

		glCreateBuffers(1, &skyBoxVBO);
		glNamedBufferData(skyBoxVBO, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glCreateBuffers(1, &skyBoxEBO);
		glNamedBufferData(skyBoxEBO, sizeof(indices), indices, GL_STATIC_DRAW);

		glCreateVertexArrays(1, &skyBoxVAO);

		glVertexArrayVertexBuffer(skyBoxVAO, 0, skyBoxVBO, 0, 3 * sizeof(float));
		glVertexArrayElementBuffer(skyBoxVAO, skyBoxEBO);

		glEnableVertexArrayAttrib(skyBoxVAO, 0);
		glVertexArrayAttribFormat(skyBoxVAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(skyBoxVAO, 0, 0);

	}

	glBindVertexArray(skyBoxVAO);
	glDepthFunc(GL_LEQUAL);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDepthFunc(GL_LESS);
	glBindVertexArray(0);
}

unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void Renderer::RenderCube()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(cubeVAO == 0)
	{
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};
		glCreateBuffers(1, &cubeVBO);
		glNamedBufferData(cubeVBO, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glCreateVertexArrays(1, &cubeVAO);
		glVertexArrayVertexBuffer(cubeVAO, 0, cubeVBO, 0, 8 * sizeof(float));
		glEnableVertexArrayAttrib(cubeVAO, 0);
		glEnableVertexArrayAttrib(cubeVAO, 1);
		glVertexArrayAttribFormat(cubeVAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribFormat(cubeVAO, 1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
		glVertexArrayAttribFormat(cubeVAO, 2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float));
		glVertexArrayAttribBinding(cubeVAO, 0, 0);
		glVertexArrayAttribBinding(cubeVAO, 1, 0);
		glVertexArrayAttribBinding(cubeVAO, 2, 0);
	}
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}