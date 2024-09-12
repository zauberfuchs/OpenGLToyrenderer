#pragma once

class VertexArray;
#include "Engine/Scene/World.h"
struct RendererContext
{


	Ref<Scene> ActiveScene;
	Shader* ActiveShader;
	Shader* PostFXShader;
	Ref<Camera> ActiveSceneCamera;

	Ref<Texture> SceneSkybox;

	Ref<VertexArray> MeshVAO;

	uint32_t MeshIndexCount;
	uint16_t MeshRenderMode;

	Ref<Material> MeshMaterial;
	Transform MeshTransform;

	std::unordered_map<EntityID, Light*> activeLights;

	int MSAA;

	GLint RenderViewport[4];
};