#pragma once
#include "Engine/Renderer/RendererData.h"
#include "Engine/Renderer/RenderPass.h"


class UniformBuffer;

struct ForwardConstants 
{
	glm::mat4 View;
	glm::mat4 Projection;
	
	glm::vec3 CameraPos;
	
	float FarPlane;
	float NearPlane;
};

class ForwardGeomPass : public RenderPass
{
public:
	ForwardGeomPass();
	void Init() override;
	void Execute(ForwardRenderContext& rendererContext) override;
	
	inline Ref<Texture> 	GetViewportTexture() 	{ return m_ViewportTexture; };
	Ref<Framebuffer> 		GetRenderTarget() 		{ return m_RenderTarget; }
	
private:
	
	Ref<Framebuffer> m_RenderTarget;
	Renderbuffer* m_Renderbuffer;
	
	UniformBuffer* m_ConstantBuffer;
	
	Ref<Texture> m_ViewportTexture;
	Ref<VertexArray> m_MeshVAO;
	uint32_t m_MeshIndexCount;
	uint16_t m_MeshRenderMode;
	
	Shader* m_Geomshader;
	ForwardConstants m_Constants;
	
};
