#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../../Engine/Scene/Camera.h"
#include "../../Engine/OpenGL/Texture.h"

class Window
{
public:
	Window(const int& width, const int& height);
	~Window();

	int InitOpenGLContext();

	void ProcessInput(GLFWwindow* window);
	void WindowRendering();
	void ImGuiRender();

	void FrameRateLimit(const unsigned int& fps);

	void NewImGuiFrame();
	void InitImGui();
	
	GLFWwindow* m_Window;
	int m_Width, m_Height;


	void DrawVec3Control(const std::string& label, glm::vec3& values, const float& vMin = 0.0f, const float& vMax = 0.0f, float resetValue = 0.0f, float columnWidth = 100.0f, bool rgbMode = false);

private:

	bool m_WireMode = false;
	bool m_hasAmbient;
	bool m_hasDiffuse;
	bool m_hasSpecular;

	Texture* m_playBTNTex;
	Texture* m_stopBTNTex;
	Texture* m_pauseBTNTex;

};

extern Camera* g_Camera;

extern const char* glsl_version;

extern const int WIN_WIDTH;
extern const int WIN_HEIGHT;

extern double lastX;
extern double lastY;

// timing
extern double deltaTime;
extern double lastFrame;
extern double lasttime;

extern bool firstMouse;
extern bool mouseButtonPressed;
