#pragma once
#include <imgui.h>

class IMesh;
class Mesh;
class Material;
class ISceneObject;
class Texture;

struct ImGuiWindowStorage
{
	ISceneObject* CurrentSceneObject;

	glm::vec3 MaterialColor;
	glm::vec3 LightColor;

	// Transform proberties
	glm::vec3 Translation;
	glm::vec3 Rotation;
	glm::vec3 Scale;

	// Phong properties
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
	float Shininess;

	// Physically Based Rendering properties
	glm::vec3 Albedo;
	float Metallic;
	float Roughness;
	float AmbientOcclusion;

	// Material Textures
	std::vector<Texture*> textures;
	bool MaterialWindowState = false;
	IMesh* SelectedMesh;
	std::unordered_set<Material*> materials;

	std::unordered_map<std::string, Texture*> ButtonTextures;

	bool HasAmbientReflection;
	bool HasDiffuseReflection;
	bool HasSpecularReflection;
	
	bool WireMode;
};

static ImGuiWindowStorage s_ImGuiData;

class ImGuiWindow
{
public:
	static void Init();
	static void NewFrame();
	static void Shutdown();

	static void RenderScenePanel();
	static void RenderTransformComponent();
	static void RenderMeshComponent();
	static void RenderLightComponent();
	static void RenderAnimationComponent();
	static void RenderWireFrameMode();
	static void RenderMSAA();
	static void RenderFpsCounter();

	static void AddUnderLine(ImColor color);
	static void DrawVec3Control(const std::string& label, glm::vec3& values, const float& vMin = 0.0f, const float& vMax = 0.0f, float resetValue = 0.0f, float columnWidth = 120.0f, bool rgbMode = false);



};

