#include "Engine/Utils/pch.h"
#include "GeomPass.h"


#include "Engine/Components/Model.h"
#include "Engine/Renderer/Renderer.h"


ForwardGeomPass::ForwardGeomPass()
{
}

void ForwardGeomPass::Init()
{
	m_ConstantBuffer = new UniformBuffer(sizeof(ForwardConstants), 1);
	m_RenderTarget = CreateRef<Framebuffer>();
	m_Renderbuffer = new Renderbuffer();
	m_RenderTarget->AttachRenderBuffer(m_Renderbuffer->GetId(), FramebufferAttachment::DepthStencil);
	
	m_ViewportTexture = CreateRef<Texture>(TextureTarget::Texture2DMultiSample);
	m_ViewportTexture->SetTexture2DSize(0, 0);
	m_ViewportTexture->CreateTexture2DStorage(TextureInternalFormat::Rgb16, false, 1);

}

void ForwardGeomPass::Execute(ForwardRenderContext& rendererContext)
{
	Renderer::BeginEvent("Forward Pass");
	m_Constants.View = rendererContext.ActiveSceneCamera->GetViewMatrix();
	m_Constants.Projection = rendererContext.ActiveSceneCamera->GetProjectionMatrix();
	m_Constants.CameraPos = rendererContext.ActiveSceneCamera->Position;
	m_Constants.FarPlane = rendererContext.ActiveSceneCamera->GetFarPlane();
	m_Constants.NearPlane = rendererContext.ActiveSceneCamera->GetNearPlane();
	m_Constants.lights[0] = *rendererContext.ActiveLights.begin()->second;
	m_ConstantBuffer->SetData(&m_Constants);
	
	uint newViewportWidth = rendererContext.ActiveSceneCamera->Viewport.Width;
	uint newViewportHeight = rendererContext.ActiveSceneCamera->Viewport.Height;
	
	
	// wenn der viewport sich verändert oder die samplesize
	if (m_ViewportTexture->GetWidth() != newViewportWidth || m_ViewportTexture->GetHeight() != newViewportHeight || m_RenderTarget->GetSampleSize() != rendererContext.MSAA)
	{
		// setzt die anzahl der Samples pro Pixel
		m_RenderTarget->SetSampleSize(rendererContext.MSAA);
		m_Renderbuffer->SetSampleSize(rendererContext.MSAA);

		// fügt die textur als color attachment dem framebuffer hinzu
		m_ViewportTexture->SetTexture2DSize(newViewportWidth, newViewportHeight);
		m_ViewportTexture->CreateTexture2DStorage(TextureInternalFormat::Rgb16, false, rendererContext.MSAA);
		m_RenderTarget->AttachColorTexture2D(m_ViewportTexture);

		// fügt den Renderbuffer als Depth/stencil attachment dem Framebuffer hinzu da wir die depth informationen brauchen
		m_Renderbuffer->CreateRenderBufferStorage(newViewportWidth, newViewportHeight, FramebufferTextureFormat::Depth32Stencil8);
	}

	m_RenderTarget->Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	for (const auto& [name, SceneObj] : rendererContext.ActiveScene->GetSceneObjects())
	{
		if (SceneObj->HasComponent<Model>())
		{
			for (const Ref<Mesh>& mesh : SceneObj->GetComponent<Model>().GetMeshes())
			{
				mesh->GetMaterial()->GetShader()->Bind();
				mesh->GetMaterial()->SetTexture(rendererContext.ActiveLights.begin()->second->GetDepthmap());
				mesh->GetMaterial()->GetShader()->SetUniformMat4f("model", SceneObj->GetComponent<Transform>().GetTransformMatrix());
				Renderer::DrawMesh(mesh, false);
			}
		}

		if(SceneObj->GetName() == "entity")
		{
			Model& m2 = SceneObj->GetComponent<Model>();

			for (const Ref<Mesh>& m : m2.GetMeshes()) 
			{
				// TODO camera is component
				Renderer::DrawMesh(m, false);
			}
		}
	}
	m_RenderTarget->Unbind();
	rendererContext.ForwardFrameBuffer = m_RenderTarget;
	rendererContext.ViewportTexture = m_ViewportTexture;
	
	Renderer::EndEvent();
}