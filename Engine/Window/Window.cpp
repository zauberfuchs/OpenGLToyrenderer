#include "../../Engine/Utils/pch.h"
#include "Window.h"

#include "../../Engine/Scene/World.h"

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

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
}



// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	g_Camera->ProcessMouseScroll(yoffset);
}

void mouseButton_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		mouseButtonPressed = true;
	}
	else
	{
		mouseButtonPressed = false;
	}
}


Window::Window(const int& width, const int& height) : m_Width(width), m_Height(height)
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Window initialization
	m_Window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "OpenGL Tutorial", NULL, NULL);
	if (!m_Window) {
		std::cout << "Erstellung des Fensters fehlgeschlagen." << std::endl;
		return -1;
	}

	// set callback functions
	// ----------------------
	glfwMakeContextCurrent(m_Window);
	glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);
	glfwSetCursorPosCallback(m_Window, mouse_callback);
	glfwSetScrollCallback(m_Window, scroll_callback);
	glfwSetMouseButtonCallback(m_Window, mouseButton_callback);

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
		g_Camera->ProcessKeyboard(CameraMovement::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		g_Camera->ProcessKeyboard(CameraMovement::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		g_Camera->ProcessKeyboard(CameraMovement::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		g_Camera->ProcessKeyboard(CameraMovement::RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		g_Camera->ProcessKeyboard(CameraMovement::UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		g_Camera->ProcessKeyboard(CameraMovement::DOWN, deltaTime);
}

void Window::WindowRendering() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// gets the size of the window in pixels
	glfwGetFramebufferSize(m_Window, &m_Width, &m_Height);
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, m_Width, m_Height);


	// per-frame time logic
	// --------------------
	double currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	// input
	// -----
	ProcessInput(m_Window);

	// inform the camera about the updated window size
	// -----------------------------------------------
	if (m_Height != 0)
	{
		g_Camera->UpdateCameraWindow(m_Width, m_Height);
	}
	glEnable(GL_DEPTH_TEST);
	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);*/
}

void Window::InitImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	// ----------------------
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	// --------------------------------
	ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	
	m_playBTNTex = new Texture("../Data/Textures/PlayButton.png", ETextureChannels::AlbedoMap);
	m_stopBTNTex = new Texture("../Data/Textures/StopButton.png", ETextureChannels::AlbedoMap);
	m_pauseBTNTex = new Texture("../Data/Textures/PauseButton.png", ETextureChannels::AlbedoMap);
}

void AddUnderLine(ImColor color)
{
	ImVec2 min = ImGui::GetItemRectMin();
	ImVec2 max = ImGui::GetItemRectMax();
	min.y = max.y;
	ImGui::GetWindowDrawList()->AddLine(
		min, max, color, 1.0f);
}

void Window::ImGuiRender()
{
	// TODO, komplett überarbeiten, Model Auswählen, meshes auswählen, Materialen Auswählen etc pp

	ImGui::Begin("Scene Panel");
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 translation(0, 0, 0);
	glm::vec3 rotation(0, 0, 0);
	glm::vec3 scale(0, 0, 0);
	std::string name;
	//static const char* current_item = NULL;
	static std::string current_item = "";
	auto sceneObjects = World::Get().GetActiveScene()->GetSceneObjects();

	ImGui::Text("Scene Objects", 20);
	AddUnderLine(ImColor(255, 255, 255, 255));
	ImGuiStyle& style = ImGui::GetStyle();
	float w = ImGui::CalcItemWidth();
	float spacing = style.ItemInnerSpacing.x;
	float button_sz = ImGui::GetFrameHeight();
	ImGui::PushItemWidth(w - spacing * 2.0f - button_sz * 2.0f);
	if (ImGui::BeginCombo("##Scene Objects", current_item.c_str()))
	{
		for (const auto [n ,sceneObject]: sceneObjects)
		{
			name = sceneObject->GetName();
			bool is_selected = current_item == name.c_str();
			if (ImGui::Selectable(name.c_str(), is_selected))
			{
				current_item = name.c_str();
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();

	ImGui::SameLine(0, style.ItemInnerSpacing.x);

	if (current_item != "")
	{
		auto s = sceneObjects.at(current_item);
		ImGui::Dummy(ImVec2(20, 20));
		translation = s->GetTransform()->GetLocalPosition();
		rotation = s->GetTransform()->GetLocalEulerAngles();
		scale = s->GetTransform()->GetLocalScale();
		color = s->GetModel().GetMeshes().begin()->second->GetMaterial()->GetColor();
		ImGui::Text("Transform", 20);
		AddUnderLine(ImColor(255, 255, 255, 255));
		ImGui::Dummy(ImVec2(1, 1));
		DrawVec3Control("Translation", translation);
		DrawVec3Control("Rotation", rotation);
		DrawVec3Control("Scale", scale);

		//ImGui::Dummy(ImVec2(20, 20));
		//ImGui::Text("Model Component", 20);
		//AddUnderLine(ImColor(255, 255, 255, 255));
		//ImGui::Dummy(ImVec2(1, 1));
		//ImGui::ColorEdit3("Model Color", (float*)&color);

		if (s->GetLight() != nullptr)
		{
			Light* l = s->GetLight();
			lightColor = l->GetColor();
			ImGui::Dummy(ImVec2(20, 20));
			ImGui::Text("Light Component", 20);
			AddUnderLine(ImColor(255, 255, 255, 255));
			ImGui::ColorEdit3("Light Color", &lightColor.x);
			l->SetColor(lightColor);
			s->GetLight()->SetPosition(translation);
		}

		if (s->GetModel().GetMeshes().size() != 0)
		{
			auto meshes = s->GetModel().GetMeshes();
			auto material = (Material*)meshes.begin()->second->GetMaterial();
			ImGui::Dummy(ImVec2(20, 20));
			ImGui::Text("Material Component", 20);
			AddUnderLine(ImColor(255, 255, 255, 255));
			ImGui::Dummy(ImVec2(1, 1));
			glm::vec3 color = material->GetColor();
			ImGui::ColorEdit3("Model Color", (float*)&color);
			material->SetColor(color);
			ImGui::Dummy(ImVec2(1, 1));
			ImGui::Text((std::string("Name: ") + material->GetName() + std::string(" Material")).c_str(), 50);
			ImGui::Dummy(ImVec2(1, 1));
			DrawVec3Control("Ambient", material->Ambient, 0, 1, 0, 100, true);
			DrawVec3Control("Diffuse", material->Diffuse, 0, 1, 0, 100, true);
			DrawVec3Control("Specular", material->Specular, 0, 1, 0, 100, true);
			ImGui::Dummy(ImVec2(1, 1));
			ImGui::DragFloat("Shininess", &material->Shininess, 1, 1, 256);

			m_hasAmbient = material->m_HasAmbient;
			m_hasDiffuse = material->m_HasDiffuse;
			m_hasSpecular = material->m_HasSpecular;
			
			ImGui::Checkbox("Toggle Ambient Reflection", &m_hasAmbient);
			if (m_hasAmbient)
			{
				material->m_HasAmbient = 1;
			}
			else
			{
				material->m_HasAmbient = 0;
			}

			ImGui::Checkbox("Toggle Diffuse Reflection", &m_hasDiffuse);
			if (m_hasDiffuse)
			{
				material->m_HasDiffuse = 1;
			}
			else
			{
				material->m_HasDiffuse = 0;
			}

			ImGui::Checkbox("Toggle Specular Reflection", &m_hasSpecular);
			if (m_hasSpecular)
			{
				material->m_HasSpecular = 1;
			}
			else
			{
				material->m_HasSpecular = 0;
			}
			
		}

		for(auto [name, mesh] : s->GetModel().GetMeshes())
		{
			mesh->GetMaterial()->GetColor() = color;
		}

		s->GetTransform()->Translate(translation, Space::Local);
		s->GetTransform()->Scale(scale, Space::Local);
		s->GetTransform()->SetRotation(rotation, Space::Local);

		if (s->GetAnimation() != nullptr)
		{
			Animation* a = s->GetAnimation();
			ImGui::Dummy(ImVec2(20, 20));
			ImGui::Text("Animation Component", 20);
			AddUnderLine(ImColor(255, 255, 255, 255));
			ImGui::Dummy(ImVec2(1, 1));
			if (ImGui::ImageButton((ImTextureID)m_playBTNTex->m_ID, { 25, 25 }, { 0, 1 }, { 1, 0 }))
			{
				a->StartAnimation();
			}
			ImGui::SameLine();
			if(ImGui::ImageButton((ImTextureID)m_pauseBTNTex->m_ID, { 25, 25 }, { 0, 1 }, { 1, 0 }))
			{
				a->PauseAnimation();
			}
			ImGui::SameLine();
			if (ImGui::ImageButton((ImTextureID)m_stopBTNTex->m_ID, { 25, 25 }, { 0, 1 }, { 1, 0 }))
			{
				a->ResetAnimation();
			}
			ImGui::Dummy(ImVec2(1, 1));
			if (ImGui::Button("Print TimeLine", { 125, 25 }))
			{
				a->PrintTimeline();
			}
		}
	}
	ImGui::Dummy(ImVec2(20, 20));
	ImGui::Checkbox("Wireframe Mode", &m_WireMode);
	if (m_WireMode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	ImGui::Dummy(ImVec2(20,20));
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();

	
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Window::DrawVec3Control(const std::string& label, glm::vec3& values, const float& vMin, const float& vMax, float resetValue, float columnWidth, bool rgbMode)
{
	ImGuiIO& io = ImGui::GetIO();

	auto boldFont = io.Fonts->Fonts[0];


	ImGui::PushID(label.c_str());
	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
	ImVec2 buttonSize = { 20.0f, 20.0f };

	ImGui::PushItemWidth(100);
	ImGui::PushItemWidth(ImGui::CalcItemWidth());
	ImGui::PushItemWidth(ImGui::CalcItemWidth());
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button(rgbMode ? "R" : "X", buttonSize))
		values.x = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##X", &values.x, 0.1f, vMin, vMax, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button(rgbMode ? "G" : "Y", buttonSize))
		values.y = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Y", &values.y, 0.1f, vMin, vMax, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button(rgbMode ? "B" : "Z", buttonSize))
		values.z = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Z", &values.z, 0.1f, vMin, vMax, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();
}

void Window::NewImGuiFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Window::FrameRateLimit(const unsigned& fps)
{
	//todo kann man geschickter machen, man schläft direkt die richtige zeit nur einmal
	while (glfwGetTime() < lasttime + 1.0 / fps) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	lasttime += 1.0 / fps;
}

Camera* g_Camera(new Camera(glm::vec3(0.0f, 20.0f, 30.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -25.0));

const char* glsl_version = "#version 330 core";

const int WIN_WIDTH = 1280;
const int WIN_HEIGHT = 1024;

double lastX = WIN_WIDTH / 2.0f;
double lastY = WIN_HEIGHT / 2.0f;

// timing
double deltaTime = 0.0f;
double lastFrame = 0.0f;
double lasttime = glfwGetTime();

bool firstMouse = true;
bool mouseButtonPressed = false;