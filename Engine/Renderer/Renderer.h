#pragma once

#include "Engine/Scene/World.h"
#include "RendererData.h"
#include "Engine/OpenGL/UniformBuffer.h"

class Mesh;

inline RendererContext g_RendererContext;





#include "Engine/RenderPasses/PostFXPass.h"
#include "Engine/RenderPasses/GeomPass.h"

class Renderer
{
public:
	void Init();
	static void Shutdown();

	void SkyboxPath();
	static void DepthPrePath();
	void GeometryPath();
	void PostFxPath();

	static void DrawMesh(Ref<Mesh> mesh, bool ShadowPass);

	static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
	static void SetClearColor(const glm::vec4& color);
	static void SetCullMode(const uint8_t& cullMode);
	static void StoreViewportSize();
	static void Clear();
	
	static void SetupConstants();

	static void RenderQuad();
	static void RenderSkyBox();
	static void RenderCube();

	
	ForwardGeomPass FGP;
	PostFXPass PFXP;
};
