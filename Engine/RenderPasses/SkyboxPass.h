#pragma once
#include "Engine/Renderer/RenderPass.h"

class SkyboxPass : public RenderPass
{
public:
	SkyboxPass();
	void Init() override;
	void Execute(ForwardRenderContext& rendererContext) override;
	
private:
	
	Shader* m_SkyboxShader;
};
