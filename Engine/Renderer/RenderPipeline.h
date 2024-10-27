#pragma once

#include "Engine/Renderer/RendererData.h"
class RenderPass;

class RenderPipeline
{
public:
	RenderPipeline(ForwardRenderContext& renderContext);
	void Init();
	void Execute();
	
	void AddRenderPass(RenderPass* renderPass) { RenderPasses.emplace_back(renderPass); };
	ForwardRenderContext GetRendererContext() { return m_RenderContext; };
private:
	
	std::vector<RenderPass*> RenderPasses;
	ForwardRenderContext& m_RenderContext;
};
