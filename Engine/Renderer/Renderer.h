#pragma once

#include "Engine/Scene/World.h"
#include "RendererData.h"
#include "Engine/OpenGL/UniformBuffer.h"

class Mesh;


#include "Engine/RenderPasses/PostFXPass.h"
#include "Engine/RenderPasses/GeomPass.h"
#include "Engine/RenderPasses/ShadowPass.h"
#include "Engine/RenderPasses/SkyboxPass.h"
#include "Engine/Renderer/RenderPipeline.h"

class Renderer
{
public:
	Renderer(); 
	void Init();
	static void Shutdown();
	
	void Render();
	
	void AddRenderPipeline(Ref<RenderPipeline> renderPipeline) { m_RenderPipeline = renderPipeline; }

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
	
	static void BeginEvent(const char* eventName);
	static void EndEvent();
	

	Ref<RenderPipeline> m_RenderPipeline;\
};
