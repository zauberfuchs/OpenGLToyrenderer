#pragma once

class RenderPass;

class RenderPipeline
{
public:
	RenderPipeline();
	void Init() override;
	void Execute(RendererContext& rendererContext) override;
	
	void AddRenderPass(RenderPass renderPass);
private:
	
	std::vector<RenderPass> RenderPasses;
};
