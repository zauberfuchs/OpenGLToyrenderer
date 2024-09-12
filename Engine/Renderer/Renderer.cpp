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
	g_RendererContext.MSAA = 1;
	g_RendererContext.PostFXShader =  World::Get().GetShader("postFX");
	g_RendererContext.ActiveScene = World::Get().GetActiveScene();
	g_RendererContext.ActiveSceneCamera = g_RendererContext.ActiveScene->GetSceneCamera();
	for(std::pair<std::string, Ref<Material>> m : World::Get().GetMaterials())
	{
		m.second->SetReflectionProbe(g_RendererContext.ActiveScene->GetReflectionProbe());
	}

	if (g_RendererContext.ActiveScene->GetSceneSkybox())
	{
		g_RendererContext.SceneSkybox = g_RendererContext.ActiveScene->GetSceneSkybox();
	}
	FGP.Init();
	PFXP.Init();
	
	g_RendererContext.activeLights = World::Get().GetEntityManager()->GetActiveComponents<Light>();

}

void Renderer::Shutdown()
{
	// pass shutdown
	//delete g_RendererContext.GeometryFramebuffer;
}

void Renderer::DepthPrePath()
{
	// safes the current viewport size.
	StoreViewportSize();

	//s_Data.ActiveSceneLight->GetFramebuffer()->Bind();
	g_RendererContext.activeLights.begin()->second->GetFramebuffer()->Bind();

	switch(g_RendererContext.activeLights.begin()->second->GetType()) {
	case LightSourceType::DirectionalLight:
		g_RendererContext.ActiveShader = World::Get().GetShader("dirLightDepthMap");
		g_RendererContext.ActiveShader->Bind();
		break;
	case LightSourceType::SpotLight:
		g_RendererContext.ActiveShader = World::Get().GetShader("spotLightDepthMap");
		g_RendererContext.ActiveShader->Bind();
		break;
	case LightSourceType::PointLight:
		g_RendererContext.ActiveShader = World::Get().GetShader("pointLightDepthMap");
		g_RendererContext.ActiveShader->Bind();
		break;
	}

	//set the Viewport size to shadow texture size
	SetViewport(0, 0, g_RendererContext.activeLights.begin()->second->GetShadowWidth(), g_RendererContext.activeLights.begin()->second->GetShadowHeight());

	//s_Data.activeLights.begin()->second->GetFramebuffer()->Bind();

	glClear(GL_DEPTH_BUFFER_BIT);
	EntityID eID = g_RendererContext.activeLights.begin()->first;
	Transform& t = World::Get().GetEntityManager()->GetComponent<Transform>(eID);
	auto shadowMatrices = g_RendererContext.activeLights.begin()->second->CreateShadowTransformMatrices(0.1f, 50.0f, t.GetLocalPosition());
	for (unsigned int i = 0; i < 6; ++i)
	{
		g_RendererContext.ActiveShader->SetUniformMat4f("shadowMatrices[" + std::to_string(i) + "]", shadowMatrices[i]);
	}
	
	g_RendererContext.ActiveShader->SetUniform1f("farPlane", g_RendererContext.ActiveSceneCamera->GetFarPlane());
	
	g_RendererContext.ActiveShader->SetUniform3f("lightPos", t.GetLocalPosition());
	
	
	for (const auto& [name, SceneObj] : g_RendererContext.ActiveScene->GetSceneObjects())
	{
		g_RendererContext.MeshTransform = SceneObj->GetComponent<Transform>(); 
		if (SceneObj->HasComponent<Model>()) 
		{
			for (const Ref<Mesh>& m : SceneObj->GetComponent<Model>().GetMeshes())
			{
				g_RendererContext.MeshVAO = m->GetVAO();
				g_RendererContext.MeshIndexCount = m->GetIndexCount();
				g_RendererContext.MeshRenderMode = m->GetRenderMode();
				g_RendererContext.MeshMaterial = nullptr;

				g_RendererContext.MeshVAO->Bind();

				DrawMesh(m, true);
			}
		}
		
	}
	g_RendererContext.activeLights.begin()->second->GetFramebuffer()->Unbind();

	// restore old Viewport size
	SetViewport(g_RendererContext.RenderViewport[0], g_RendererContext.RenderViewport[1], g_RendererContext.RenderViewport[2], g_RendererContext.RenderViewport[3]);
}

void Renderer::GeometryPath()
{

	FGP.Execute(g_RendererContext);
}

void Renderer::SkyboxPath()
{
	if (g_RendererContext.SceneSkybox != nullptr)
	{
		g_RendererContext.ActiveShader = World::Get().GetShader("skybox");
		g_RendererContext.ActiveShader->Bind();
		g_RendererContext.ActiveShader->SetUniformMat4f("view", glm::mat4(glm::mat3(g_RendererContext.ActiveSceneCamera->GetViewMatrix())));// this removes the translation from the view matrix
		g_RendererContext.ActiveShader->SetUniformMat4f("projection", g_RendererContext.ActiveSceneCamera->GetProjectionMatrix());
		g_RendererContext.SceneSkybox->Bind(0);
		RenderSkyBox();
	}
	FGP.ForwardGeomFB->Unbind();
}

void Renderer::PostFxPath()
{
	PFXP.SetViewportTexture(FGP.GetViewportTexture());
	PFXP.Execute(g_RendererContext);
}

void Renderer::DrawMesh(Ref<Mesh> mesh, bool ShadowPass)
{
	Shader* shader = mesh->GetMaterial()->GetShader();
	shader->Bind();
	mesh->GetVAO()->Bind();
	
	mesh->GetMaterial()->SetTexture(g_RendererContext.activeLights.begin()->second->GetDepthmap());
	mesh->GetMaterial()->SetupUniforms();
	shader->SetUniformMat4f("model", g_RendererContext.MeshTransform.GetTransformMatrix());
	
	glDrawElements(mesh->GetRenderMode(), mesh->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	glViewport(x, y, width, height);
}

void Renderer::StoreViewportSize()
{
	glGetIntegerv(GL_VIEWPORT, g_RendererContext.RenderViewport);
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

void Renderer::RenderQuad()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	uint Vao;
	glCreateVertexArrays(1, &Vao);
	glBindVertexArray(Vao);
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