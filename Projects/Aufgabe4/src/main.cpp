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

	auto skyboxShader = new Shader("skyboxShader", "../Data/skyboxShader.vert", "../Data/skyboxShader.frag");
	auto lightShader = new Shader("lightShader", "../Data/lightShader.vert", "../Data/lightShader.frag");
	auto simpleMeshShader = new Shader("simpleMeshShader", "../Data/simpleMeshShader.vert", "../Data/simpleMeshShader.frag");

	auto gridTexture = new Texture("../Data/Textures/uvw.png", ETextureChannels::DiffuseMap);

	World::Get().AddShader(skyboxShader);
	World::Get().AddShader(lightShader);
	World::Get().AddShader(simpleMeshShader);

	auto textureMaterial = new Material("grid");
	textureMaterial->Ambient = glm::vec3(0.3f, 0.3f, 0.3f);
	textureMaterial->Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	textureMaterial->Specular = glm::vec3(0.5f, 0.5f, 0.5f);
	textureMaterial->Shininess = 32.0f;
	textureMaterial->m_Color = glm::vec3(1.0f);
	textureMaterial->SetShader(simpleMeshShader);
	textureMaterial->SetTexture(gridTexture);
	textureMaterial->SetReflections(ReflectionType::Phong);

	auto whiteMaterial = new Material("white");
	whiteMaterial->Ambient = glm::vec3(0.3f, 0.3f, 0.3f);
	whiteMaterial->Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	whiteMaterial->Specular = glm::vec3(0.5f, 0.5f, 0.5f);
	whiteMaterial->Shininess = 32.0f;
	whiteMaterial->m_Color = glm::vec3(1.0f);
	whiteMaterial->SetShader(lightShader);

	auto blueMaterial = new Material("blue");
	blueMaterial->Ambient = glm::vec3(0.3f, 0.3f, 0.3f);
	blueMaterial->Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	blueMaterial->Specular = glm::vec3(0.5f, 0.5f, 0.5f);
	blueMaterial->Shininess = 32.0f;
	blueMaterial->m_Color = glm::vec3(0.2f, 0.2f, 0.8f);
	blueMaterial->SetShader(simpleMeshShader);
	blueMaterial->SetReflections(ReflectionType::Ambient);

	auto redMaterial = new Material("red");
	redMaterial->Ambient = glm::vec3(0.3f, 0.3f, 0.3f);
	redMaterial->Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	redMaterial->Specular = glm::vec3(0.5f, 0.5f, 0.5f);
	redMaterial->Shininess = 32.0f;
	redMaterial->m_Color = glm::vec3(0.8f, 0.2f, 0.2f);
	redMaterial->SetShader(simpleMeshShader);
	redMaterial->SetReflections(ReflectionType::Diffuse);

	auto tealMaterial = new Material("teal");
	tealMaterial->Ambient = glm::vec3(0.3f, 0.3f, 0.3f);
	tealMaterial->Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	tealMaterial->Specular = glm::vec3(0.5f, 0.5f, 0.5f);
	tealMaterial->Shininess = 32.0f;
	tealMaterial->m_Color = glm::vec3(0.2f, 0.8f, 0.8f);
	tealMaterial->SetShader(simpleMeshShader);
	tealMaterial->SetReflections(ReflectionType::Specular);

	auto cubeLight = new Light("cubeLight");
	cubeLight->Type = LightSourceType::PointLight;
	cubeLight->Position = glm::vec3(1.2f, 1.0f, 2.0f);
	cubeLight->Color = glm::vec3(1.0f, 1.0f, 1.0f);

	///////////////////////////////////////////////////////////////////////////////
	// Create geometry
	///////////////////////////////////////////////////////////////////////////////

	SceneObject ground("ground");
	SceneObject quad("quad");
	SceneObject cube("cube");
	SceneObject lightCube("lightCube");
	SceneObject secondQuad("secondQuad");

	Skybox skybox("universe");

	activeScene->AddRootChild(&ground);
	activeScene->AddRootChild(&quad);
	activeScene->AddRootChild(&lightCube);
	activeScene->AddRootChild(&cube);
	activeScene->AddRootChild(&secondQuad);

	ground.AddModel(new Model(std::string("ground")));
	cube.AddModel(new Model(std::string("cube")));
	quad.AddModel(new Model(std::string("quad")));
	lightCube.AddModel(new Model(std::string("lightCube")));
	lightCube.AddLight(cubeLight);
	secondQuad.AddModel(new Model(std::string("secondQuad")));

	ground.GetModel().AddMesh(new Plane("ground"));
	cube.GetModel().AddMesh(new StudentCube("cube"));
	quad.GetModel().AddMesh(new StudentCube("quad"));
	lightCube.GetModel().AddMesh(new StudentCube("lightCube"));
	secondQuad.GetModel().AddMesh(new StudentCube("secondQuad"));

	activeScene->GetSceneObject("ground")->GetModel().GetMesh("ground")->SetMaterial(textureMaterial);
	activeScene->GetSceneObject("cube")->GetModel().GetMesh("cube")->SetMaterial(redMaterial);
	activeScene->GetSceneObject("quad")->GetModel().GetMesh("quad")->SetMaterial(tealMaterial);
	activeScene->GetSceneObject("secondQuad")->GetModel().GetMesh("secondQuad")->SetMaterial(blueMaterial);
	activeScene->GetSceneObject("lightCube")->GetModel().GetMesh("lightCube")->SetMaterial(whiteMaterial);

	lightCube.GetTransform()->Translate(cubeLight->Position, Space::Local);
	lightCube.GetTransform()->Scale(glm::vec3(0.2f), Space::Local);



	ground.GetTransform()->Scale(glm::vec3(20.f), Space::Local);

	glm::mat4 m;

	m = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.5f, 0.0f));
	m = glm::scale(m, glm::vec3(2.0f));
	m = glm::rotate(m, glm::radians(45.0f), X_ACHSE);

	

	secondQuad.GetTransform()->SetTransformMatrix(m);

	///////////////////////////////////////////////////////////////////////////////
	// Setup Scene
	///////////////////////////////////////////////////////////////////////////////

	window->InitImGui();

	activeScene->SetSceneSkybox(&skybox);
	activeScene->SetSceneCamera(g_Camera);
	activeScene->AddSceneLight(cubeLight);


	///////////////////////////////////////////////////////////////////////////////
	// main Rendering loop
	///////////////////////////////////////////////////////////////////////////////

	while (!glfwWindowShouldClose(window->m_Window)) {
		window->WindowRendering();
		window->NewImGuiFrame();

		activeScene->UpdateScene();

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
	delete simpleMeshShader;
	delete gridTexture;
	delete lightShader;
	delete skyboxShader;
	delete activeScene;
	delete cubeLight;

	return error;
}

