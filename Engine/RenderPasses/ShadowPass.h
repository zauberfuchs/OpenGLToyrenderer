#pragma once
#include "Engine/Renderer/RendererData.h"
#include "Engine/Renderer/RenderPass.h"

class ShadowPass : public RenderPass
{
public:
	ShadowPass();
	void Init() override;
	void Execute(ForwardRenderContext& rendererContext) override;
	
	void SetViewportTexture(Ref<Texture> viewportTexture) { m_ViewportTexture = viewportTexture; };
private:
	
	Shader* m_ShadowShader;
	std::unordered_map<EntityID, Light*> m_Lights;
	Ref<Texture> m_ViewportTexture;
};
