#pragma once
#include <imgui.h>

class Texture;

struct ImGuiWindowStorage
{
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
	static void Render();
	static void Shutdown();

	

	static void AddUnderLine(ImColor color);
	static void DrawVec3Control(const std::string& label, glm::vec3& values, const float& vMin = 0.0f, const float& vMax = 0.0f, float resetValue = 0.0f, float columnWidth = 100.0f, bool rgbMode = false);



};

