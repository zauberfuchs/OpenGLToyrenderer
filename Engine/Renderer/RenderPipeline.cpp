#include "Engine/Utils/pch.h"
#include "RenderPipeline.h"

#include "Engine/Renderer/Renderer.h"


RenderPipeline::RenderPipeline(ForwardRenderContext& renderContext)
	: m_RenderContext(renderContext)
{
	
}

void RenderPipeline::Init()
{
	for (RenderPass* rp : RenderPasses)
		rp->Init();
}

void RenderPipeline::Execute()
{
	for (RenderPass* rp : RenderPasses)
		rp->Execute(m_RenderContext);
}