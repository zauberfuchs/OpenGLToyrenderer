#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Mesh.h"
#include "Plane.h"
#include "Aufgabe1/StudentCube.h"
#include "Skybox.h"
#include "SceneObject.h"
#include "Window.h"
#include "Scene.h"
#include "Model.h"
#include "World.h"

int main() {

	auto window = new Window(1250, 680);
	int error = window->InitOpenGLContext();

	World::Get().SetActiveScene(new Scene("Main Scene"));

	Scene* activeScene = World::Get().GetActiveScene();

	///////////////////////////////////////////////////////////////////////////////
	// Setup Shader / Materials / Lights
	///////////////////////////////////////////////////////////////////////////////

	auto lightShader = new Shader("lightShader", "../Data/lightShader.vert", "../Data/lightShader.frag");
	auto skyboxShader = new Shader("skyboxShader", "../Data/skyboxShader.vert", "../Data/skyboxShader.frag");
	auto simpleColorShader = new Shader("simpleColorShader", "../Data/simpleColorShader.vert", "../Data/simpleColorShader.frag");

	World::Get().AddShader(lightShader);
	World::Get().AddShader(skyboxShader);
	World::Get().AddShader(simpleColorShader);

	auto whiteMaterial = new Material("white");
	whiteMaterial->Ambient = glm::vec3(0.3f, 0.3f, 0.3f);
	whiteMaterial->Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	whiteMaterial->Specular = glm::vec3(0.5f, 0.5f, 0.5f);
	whiteMaterial->Shininess = 32.0f;
	whiteMaterial->m_ObjectColor = glm::vec3(1.0f);
	whiteMaterial->SetShader(lightShader);

	auto greyMaterial = new Material("grey");
	greyMaterial->Ambient = glm::vec3(0.3f, 0.3f, 0.3f);
	greyMaterial->Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	greyMaterial->Specular = glm::vec3(0.5f, 0.5f, 0.5f);
	greyMaterial->Shininess = 32.0f;
	greyMaterial->m_ObjectColor = glm::vec3(0.5f, 0.5f, 0.5f);
	greyMaterial->SetShader(simpleColorShader);
	greyMaterial->SetReflections(ReflectionType::Ambient);

	auto redMaterial = new Material("red");
	redMaterial->Ambient = glm::vec3(0.3f, 0.3f, 0.3f);
	redMaterial->Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	redMaterial->Specular = glm::vec3(0.5f, 0.5f, 0.5f);
	redMaterial->Shininess = 32.0f;
	redMaterial->m_ObjectColor = glm::vec3(0.8f, 0.2f, 0.2f);
	redMaterial->SetShader(simpleColorShader);
	redMaterial->SetReflections(ReflectionType::Diffuse);

	auto tealMaterial = new Material("teal");
	tealMaterial->Ambient = glm::vec3(0.3f, 0.3f, 0.3f);
	tealMaterial->Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	tealMaterial->Specular = glm::vec3(0.5f, 0.5f, 0.5f);
	tealMaterial->Shininess = 32.0f;
	tealMaterial->m_ObjectColor = glm::vec3(0.2f, 0.8f, 0.8f);
	tealMaterial->SetShader(simpleColorShader);
	tealMaterial->SetReflections(ReflectionType::Specular);

	auto cubeLight = new Light("cubeLight");
	cubeLight->Type = LightSourceType::PointLight;
	cubeLight->Position = glm::vec3(1.2f, 4.0f, 2.0f);
	cubeLight->Constant = 1.0f;
	cubeLight->Linear = 0.9f;
	cubeLight->Quadratic = 0.032f;
	cubeLight->Color = glm::vec3(1.0f);

	///////////////////////////////////////////////////////////////////////////////
	// Create geometry
	///////////////////////////////////////////////////////////////////////////////

	SceneObject ground("ground");
	SceneObject quad("quad");
	SceneObject cube("cube");
	SceneObject lightCube("lightCube");

	Skybox skybox("universe");

	activeScene->AddRootChild(&ground);
	activeScene->AddRootChild(&quad);
	activeScene->AddRootChild(&lightCube);
	activeScene->AddRootChild(&cube);

	ground.AddModel(new Model(std::string("ground")));
	cube.AddModel(new Model(std::string("cube")));
	quad.AddModel(new Model(std::string("quad")));
	lightCube.AddModel(new Model(std::string("lightCube")));

	ground.GetModel().AddMesh(new Plane("ground"));
	cube.GetModel().AddMesh(new StudentCube("cube"));
	quad.GetModel().AddMesh(new StudentCube("quad"));
	lightCube.GetModel().AddMesh(new StudentCube("lightCube"));

	activeScene->GetSceneObject("ground")->GetModel().GetMesh("ground")->SetMaterial(greyMaterial);
	activeScene->GetSceneObject("cube")->GetModel().GetMesh("cube")->SetMaterial(redMaterial);
	activeScene->GetSceneObject("quad")->GetModel().GetMesh("quad")->SetMaterial(tealMaterial);
	activeScene->GetSceneObject("lightCube")->GetModel().GetMesh("lightCube")->SetMaterial(whiteMaterial);
	
	lightCube.GetTransform()->Translate(cubeLight->Position, Space::Local);
	lightCube.GetTransform()->Scale(glm::vec3(0.2f), Space::Local);

	ground.GetTransform()->Scale(glm::vec3(20.f), Space::Local);

	///////////////////////////////////////////////////////////////////////////////
	// Setup Scene
	///////////////////////////////////////////////////////////////////////////////

	window->InitImGui();

	activeScene->SetSceneCamera(g_Camera);
	activeScene->SetSceneSkybox(&skybox);
	activeScene->AddSceneLight(cubeLight);

	///////////////////////////////////////////////////////////////////////////////
	// main Rendering loop
	///////////////////////////////////////////////////////////////////////////////

	while (!glfwWindowShouldClose(window->m_Window)) {
		window->WindowRendering();
		window->NewImGuiFrame();
		
		activeScene->RenderScene();
		window->ImGuiRender();

		glfwSwapBuffers(window->m_Window);
		glfwPollEvents();
	}

	// cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	delete g_Camera;
	delete window;
	delete simpleColorShader;
	delete lightShader;
	delete skyboxShader;
	delete activeScene;
	delete cubeLight;
	
	return error;
}