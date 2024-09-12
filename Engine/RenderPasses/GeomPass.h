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

class ForwardGeomPass : RenderPass
{
public:
	ForwardGeomPass();
	void Init() override;
	void Execute(RendererContext& rendererContext) override;
	
	inline Ref<Texture> GetViewportTexture() { return ViewportTexture; };
	
	Framebuffer* ForwardGeomFB;
private:
	
	Renderbuffer* ForwardGeomRB;
	
	UniformBuffer* m_ConstantBuffer;
	
	//Shader m_ActiveShader;
	Ref<Texture> ViewportTexture;
	Ref<VertexArray> m_MeshVAO;
	uint32_t m_MeshIndexCount;
	uint16_t m_MeshRenderMode;
	
	ForwardConstants m_Constants;
	
};
