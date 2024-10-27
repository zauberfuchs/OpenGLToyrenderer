#pragma once
#include "Engine/Scene/World.h"

class VertexArray;

struct ForwardRenderContext
{
	Ref<Scene> 			ActiveScene;
	Ref<Camera> 		ActiveSceneCamera;
	Ref<Texture> 		SceneSkybox;
	Ref<Texture> 		ViewportTexture;
	
	Ref<Framebuffer> 	ForwardFrameBuffer;
	
	std::unordered_map<EntityID, Light*> ActiveLights;

	int MSAA;

	GLint RenderViewport[4];
};