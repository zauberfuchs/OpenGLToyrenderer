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
	ForwardGeomFB = new Framebuffer();
	ForwardGeomRB = new Renderbuffer();
	ForwardGeomFB->AttachRenderBuffer(ForwardGeomRB->GetId(), FramebufferAttachment::DepthStencil);
	
	ViewportTexture = CreateRef<Texture>(TextureTarget::Texture2DMultiSample);
	ViewportTexture->SetTexture2DSize(0, 0);
	ViewportTexture->CreateTexture2DStorage(TextureInternalFormat::Rgb16, false, 1);

}

void ForwardGeomPass::Execute(RendererContext& rendererContext)
{
	m_Constants.View = rendererContext.ActiveSceneCamera->GetViewMatrix();
	m_Constants.Projection = rendererContext.ActiveSceneCamera->GetProjectionMatrix();
	m_Constants.CameraPos = rendererContext.ActiveSceneCamera->Position;
	m_Constants.FarPlane = rendererContext.ActiveSceneCamera->GetFarPlane();
	m_Constants.NearPlane = rendererContext.ActiveSceneCamera->GetNearPlane();
	m_ConstantBuffer->SetData(&m_Constants);
	
	int newViewportWidth = rendererContext.ActiveSceneCamera->Viewport.Width;
	uint newViewportHeight = rendererContext.ActiveSceneCamera->Viewport.Height;
	
	
	// wenn der viewport sich verändert oder die samplesize
	if (ViewportTexture->GetWidth() != newViewportWidth || ViewportTexture->GetHeight() != newViewportHeight || ForwardGeomFB->GetSampleSize() != rendererContext.MSAA)
	{
		// setzt die anzahl der Samples pro Pixel
		ForwardGeomFB->SetSampleSize(rendererContext.MSAA);
		ForwardGeomRB->SetSampleSize(rendererContext.MSAA);

		// fügt die textur als color attachment dem framebuffer hinzu
		ViewportTexture->SetTexture2DSize(newViewportWidth, newViewportHeight);
		ViewportTexture->CreateTexture2DStorage(TextureInternalFormat::Rgb16, false, rendererContext.MSAA);
		ForwardGeomFB->AttachColorTexture2D(ViewportTexture);

		// fügt den Renderbuffer als Depth/stencil attachment dem Framebuffer hinzu da wir die depth informationen brauchen
		ForwardGeomRB->CreateRenderBufferStorage(newViewportWidth, newViewportHeight, FramebufferTextureFormat::Depth32Stencil8);
	}

	ForwardGeomFB->Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	for (const auto& [name, SceneObj] : rendererContext.ActiveScene->GetSceneObjects())
	{
		rendererContext.MeshTransform = SceneObj->GetComponent<Transform>();
		if (SceneObj->HasComponent<Model>())
		{
			for (const Ref<Mesh>& mesh : SceneObj->GetComponent<Model>().GetMeshes())
			{
				Renderer::DrawMesh(mesh, true);
			}
		}

		if(SceneObj->GetName() == "entity")
		{
			Model& m2 = SceneObj->GetComponent<Model>();

			for (const Ref<Mesh>& m : m2.GetMeshes()) 
			{
				// TODO camera is component
				Renderer::DrawMesh(m, true);
			}
		}
	}
	
}