#include "Engine/Utils/pch.h"
#include "Renderer.h"

#include "Engine/Components/Model.h"


Renderer::Renderer()
{}

void Renderer::Init()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_DEPTH_TEST);
	StoreViewportSize();
	
	for(std::pair<std::string, Ref<Material>> m : World::Get().GetMaterials())
	{
		m.second->SetReflectionProbe(m_RenderPipeline->GetRendererContext().ActiveScene->GetReflectionProbe());
	}
	m_RenderPipeline->Init();

}

void Renderer::Shutdown()
{
}

void Renderer::Render()
{
	m_RenderPipeline->Execute();
}

void Renderer::DrawMesh(Ref<Mesh> mesh, bool ShadowPass)
{
	if (!ShadowPass)
	{
		mesh->GetMaterial()->SetupUniforms();
	}
	
	mesh->GetVAO()->Bind();
	
	glDrawElements(mesh->GetRenderMode(), mesh->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	glViewport(x, y, width, height);
}

void Renderer::StoreViewportSize()
{
	//glGetIntegerv(GL_VIEWPORT, g_RendererContext.RenderViewport);
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
	uint32 Vao;
	glCreateVertexArrays(1, &Vao);
	glBindVertexArray(Vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void Renderer::RenderSkyBox()
{
	uint32 skyBoxVAO = 0;
	uint32 skyBoxVBO = 0;
	uint32 skyBoxEBO = 0;
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

		uint32 indices[] =
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

void Renderer::RenderCube()
{
	uint32 cubeVAO = 0;
	uint32 cubeVBO = 0;
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