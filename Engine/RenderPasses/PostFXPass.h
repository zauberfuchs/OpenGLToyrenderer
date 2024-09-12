#pragma once
#include "Engine/Renderer/RendererData.h"
#include "Engine/Renderer/RenderPass.h"


class PostFXPass : RenderPass
{
public:
	PostFXPass();
	void Init() override;
	void Execute(RendererContext& rendererContext) override;
	
	void RenderQuad();
	void SetViewportTexture(Ref<Texture> viewportTexture) { m_ViewportTexture = viewportTexture; };
private:
	
	Shader* m_PostFXShader;
	Ref<Texture> m_ViewportTexture;
};
