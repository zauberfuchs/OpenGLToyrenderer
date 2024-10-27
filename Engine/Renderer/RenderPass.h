#pragma once
#include "RendererData.h"


class RenderPass
{
public:
	virtual ~RenderPass() {};
	
	virtual void Init() = 0;
	
	virtual void Execute(ForwardRenderContext& rendererContext) = 0;
	
	
	std::vector<RenderPass*> m_Dependencys;
};