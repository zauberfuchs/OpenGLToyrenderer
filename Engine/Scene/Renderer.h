#pragma once


#include "World.h"

class VertexArray;

struct RendererStorage
{
	Framebuffer* GeometryFramebuffer;
	Renderbuffer* GeometryRenderbuffer;

	Scene* ActiveScene;
	Shader* ActiveShader;
	Shader* PostFXShader;
	Camera* ActiveSceneCamera;

	Skybox* SceneSkybox;

	VertexArray* MeshVAO;

	uint32_t MeshIndexCount;
	uint16_t MeshRenderMode;

	Material* MeshMaterial;
	Transform* MeshTransform;

	Light* ActiveSceneLight;

	int* MSAA;

	GLint RenderViewport[4];
};

inline RendererStorage s_Data;

class Renderer
{
public:
	static void Init();
	static void Shutdown();

	static void SkyboxPath();
	static void DepthPrePath();
	static void GeometryPath();
	static void PostFxPath();

	static void DrawMesh();

	static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
	static void SetClearColor(const glm::vec4& color);
	static void SetCullMode(const uint8_t& cullMode);
	static void UpdateViewport();
	static void Clear();

	static void RenderQuad();
};
