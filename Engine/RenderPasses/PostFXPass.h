#pragma once
#include "Engine/Renderer/RendererData.h"
#include "Engine/Renderer/RenderPass.h"


class PostFXPass : public RenderPass
{
public:
	PostFXPass();
	void Init() override;
	void Execute(ForwardRenderContext& rendererContext) override;
	
	void SetViewportTexture(Ref<Texture> viewportTexture) { m_ViewportTexture = viewportTexture; };
private:
	
	Shader* m_PostFXShader;
	Ref<Texture> m_ViewportTexture;
};
