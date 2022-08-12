#include "../../Engine/Utils/pch.h"
#include "Window.h"

#include "../../Engine/Scene/World.h"
#include "../../Engine/Scene/Renderer.h"


Window::Window(const int& width, const int& height) : m_Width(width), m_Height(height), m_DeltaTime(0.0), m_LastFrame(0.0), m_LastTime(glfwGetTime())
{
};

Window::~Window()
{
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

int Window::InitOpenGLContext()
{
	// GLFW Initialisierung
	// --------------------
	if (!glfwInit()) {
		std::cout << "Initalisierung von GLFW3 fehlgeschlagen." << std::endl;
		return -1;
	}

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.2
	// -------------------------------------------
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

	// Window initialization
	m_Window = glfwCreateWindow(m_Width, m_Height, "OpenGL Tutorial", NULL, NULL);
	if (!m_Window) {
		std::cout << "Erstellung des Fensters fehlgeschlagen." << std::endl;
		return -1;
	}

	glfwMakeContextCurrent(m_Window);

	// set callback functions
	// ----------------------
	glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
	{
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
	});
	
	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
	{
		static double lastX;
		static double lastY;
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		double xoffset = xpos - lastX;
		double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

		lastX = xpos;
		lastY = ypos;
		if (mouseButtonPressed)
		{
			g_Camera->ProcessMouseMovement(xoffset, yoffset);
		}
	});
	
	glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
	{
			g_Camera->ProcessMouseScroll(yoffset);
	});
	
	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
	{
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		{
			mouseButtonPressed = true;
		}
		else
		{
			mouseButtonPressed = false;
		}
	});

	// tell GLFW to capture our mouse
	// ------------------------------
	glfwSetInputMode(m_Window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_CURSOR_DISABLED);


	// Initialize OpenGL loader
	// ------------------------
	bool err = glewInit() != GLEW_OK;

	std::cout << glGetString(GL_VERSION) << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


	// background color
	// ----------------
	glClearColor(0.15f, 0.15f, 0.15f, 1.0f);

	// enable the depth buffer
	// -----------------------
	glEnable(GL_DEPTH_TEST);

	return 0;
}

void Window::ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		g_Camera->ProcessKeyboard(CameraMovement::FORWARD, m_DeltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		g_Camera->ProcessKeyboard(CameraMovement::BACKWARD, m_DeltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		g_Camera->ProcessKeyboard(CameraMovement::LEFT, m_DeltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		g_Camera->ProcessKeyboard(CameraMovement::RIGHT, m_DeltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		g_Camera->ProcessKeyboard(CameraMovement::UP, m_DeltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		g_Camera->ProcessKeyboard(CameraMovement::DOWN, m_DeltaTime);
}

void Window::WindowRendering() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// gets the size of the window in pixels
	glfwGetFramebufferSize(m_Window, &m_Width, &m_Height);
	// Specify the viewport of OpenGL in the Window
	glViewport(0, 0, m_Width, m_Height);


	// per-frame time logic
	// --------------------
	double currentFrame = glfwGetTime();
	m_DeltaTime = currentFrame - m_LastFrame;
	m_LastFrame = currentFrame;

	// input
	// -----
	ProcessInput(m_Window);

	// inform the camera about the updated window size
	// -----------------------------------------------
	if (m_Height != 0)
	{
		g_Camera->UpdateCameraWindow(m_Width, m_Height);
	}
}

void Window::FrameRateLimit(const unsigned& fps)
{
	//todo kann man geschickter machen, man schläft direkt die richtige zeit nur einmal
	while (glfwGetTime() < m_LastTime + 1.0 / fps) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	m_LastTime += 1.0 / fps;
}