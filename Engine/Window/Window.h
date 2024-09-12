#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Engine/OpenGL/Shader.h"
#include "Engine/Scene/Camera.h"

class Window
{
public:
	Window(const int& width, const int& height);
	~Window();

	int InitOpenGLContext();

	void ProcessInput(GLFWwindow* window);
	void WindowRendering();

	void FrameRateLimit(const unsigned int& fps);
	
	
	GLFWwindow* m_Window;
	int m_Width, m_Height;

	double m_LastX, m_LastY;

	double m_DeltaTime, m_LastFrame, m_LastTime;


};

inline Ref<Camera> g_Camera;

inline const char* glsl_version = "#version 330 core";

inline bool firstMouse = true;
inline bool mouseButtonPressed = false;
