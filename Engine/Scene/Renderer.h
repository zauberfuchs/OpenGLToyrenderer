#pragma once


#include "World.h"

class VertexArray;

struct RendererStorage
{
	Framebuffer* GeometryFramebuffer;
	Renderbuffer* GeometryRenderbuffer;
	Ref<Texture> ViewportTexture;


	Ref<Scene> ActiveScene;
	Shader* ActiveShader;
	Shader* PostFXShader;
	Ref<Camera> ActiveSceneCamera;

	Ref<Texture> SceneSkybox;

	Ref<VertexArray> MeshVAO;

	uint32_t MeshIndexCount;
	uint16_t MeshRenderMode;

	Ref<Material> MeshMaterial;
	Transform MeshTransform;

	std::unordered_map<EntityID, Light*> activeLights;

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
	static void StoreViewportSize();
	static void Clear();

	static void RenderQuad();
	static void RenderSkyBox();
	static void RenderCube();
};
