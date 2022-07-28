#include "../../Engine/Utils/pch.h"

#include "../../Engine/Components/Model.h"
#include "../../Engine/Scene/StudentCube.h"
#include "../../Engine/Scene/Skybox.h"
#include "../../Engine/Scene/Sphere.h"
#include "../../Engine/Window/Window.h"

#define FPS 30

int main() {

	auto window = new Window(1250, 680);
	int error = window->InitOpenGLContext();

	World::Get().SetActiveScene(new Scene("Main Scene"));

	Scene* activeScene = World::Get().GetActiveScene();
	activeScene->SetSceneCamera(g_Camera);

	///////////////////////////////////////////////////////////////////////////////
	// Setup Shader / Materials / Lights
	///////////////////////////////////////////////////////////////////////////////
	ShaderLoader sLoader;
	sLoader.LoadShaderFolder("../Data/Shaders/");

	Skybox skybox("universe");
	
	ReflectionProbe probeOne(1024, 1024);
	probeOne.CreateReflectionMapFromHDR("../Data/Textures/Hdr/Newport_Loft_Ref.hdr");
	skybox.SetId(probeOne.GetReflectionMap());
	probeOne.Create();

	activeScene->SetReflectionProbe(&probeOne);

	MaterialLoader mLoader;
	mLoader.LoadMaterialFolder("../Data/Textures/Materials/");

	//Todo: phongmaterial
	auto whiteMaterial = new Material("white");
	whiteMaterial->Ambient = glm::vec3(0.3f, 0.3f, 0.3f);
	whiteMaterial->Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	whiteMaterial->Specular = glm::vec3(0.5f, 0.5f, 0.5f);
	whiteMaterial->Shininess = 32.0f;
	whiteMaterial->m_Color = glm::vec3(1.0f);
	whiteMaterial->SetShader(World::Get().GetShader("lightShader"));

	auto cubeLight = new Light("cubeLight");
	cubeLight->Type = LightSourceType::DirectionalLight;
	cubeLight->Position = glm::vec3(10.2f, 4.0f, 2.0f);
	cubeLight->Constant = 1.0f;
	cubeLight->Linear = 0.9f;
	cubeLight->Quadratic = 0.032f;
	cubeLight->Color = glm::vec3(1.0f);

	///////////////////////////////////////////////////////////////////////////////
	// Create geometry
	///////////////////////////////////////////////////////////////////////////////

	const unsigned int maxRows = 3;
	const unsigned int maxColumns = 3;
	
	SceneObject* shaderBalls[maxRows][maxColumns];


	int i = 1;
	for (int row = 0; row < maxRows; row++)
	{
		for (int col = 0; col < maxColumns; col++)
		{
			shaderBalls[row][col] = new SceneObject("shaderball" + std::to_string(i));
			i++;
		}
	}

	for (int row = 0; row < maxRows; row++)
	{
		for (int col = 0; col < maxColumns; col++)
		{
			activeScene->AddRootChild(shaderBalls[row][col]);
			shaderBalls[row][col]->AddModel(new Model("../Data/Models/test-models/ShaderballUnreal.obj"));
			shaderBalls[row][col]->GetTransform()->Translate(glm::vec3(col * 5.0f, 0.0f, row * 5.0f), Space::Local);
			shaderBalls[row][col]->GetTransform()->Scale(glm::vec3(0.01f, 0.01f, 0.01f), Space::Local);
		}
	}
	srand(time(NULL));
	for(const auto& s : activeScene->GetSceneObjects())
	{
		if (s.first.find("shaderball") == 0)
		{

			s.second->GetModel().GetMesh(0)->SetMaterial(World::Get().GetMaterial(rand() % 17));
			s.second->GetModel().GetMesh(1)->SetMaterial(World::Get().GetMaterial(rand() % 17));
		}
	}


	///////////////////////////////////////////////////////////////////////////////
	// ground
	///////////////////////////////////////////////////////////////////////////////

	SceneObject ground("ground");
	activeScene->AddRootChild(&ground);
	ground.AddModel(new Model(std::string("ground")));
	ground.GetModel().AddMesh(new StudentCube("ground"));
	activeScene->GetSceneObject("ground")->GetModel().GetMesh(0)->SetMaterial(World::Get().GetMaterial("ceramictile"));
	ground.GetTransform()->Scale(glm::vec3(30.00f, .25f, 30.00f), Space::Local);
	ground.GetTransform()->Translate(glm::vec3(10.0f, 0.0f, 4.0f), Space::Local);

	///////////////////////////////////////////////////////////////////////////////
	// Light
	///////////////////////////////////////////////////////////////////////////////
	
	SceneObject lightCube("lightCube");
	activeScene->AddRootChild(&lightCube);
	lightCube.AddModel(new Model(std::string("lightCube")));
	lightCube.GetModel().AddMesh(new Sphere("Sphere", 256));
	lightCube.GetModel().AddMesh(new Sphere("Sphere", 256));
	lightCube.AddLight(cubeLight);
	activeScene->GetSceneObject("lightCube")->GetModel().GetMesh(0)->SetMaterial(whiteMaterial);

	lightCube.GetTransform()->Translate(cubeLight->Position, Space::Local);
	lightCube.GetTransform()->Scale(glm::vec3(0.2f), Space::Local);

	

	///////////////////////////////////////////////////////////////////////////////
	// Setup Scene
	///////////////////////////////////////////////////////////////////////////////

	window->InitImGui();

	activeScene->SetSceneSkybox(&skybox);
	activeScene->AddSceneLight(cubeLight);

	///////////////////////////////////////////////////////////////////////////////
	// main Rendering loop
	///////////////////////////////////////////////////////////////////////////////

	double lasttime = glfwGetTime();



	while (!glfwWindowShouldClose(window->m_Window)) {

		activeScene->UpdateScene();
		window->WindowRendering();
		window->NewImGuiFrame();

		// kann man geschickter machen, man schläft direkt die richtige zeit nur einmal
		while (glfwGetTime() < lasttime + 1.0 / FPS) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		lasttime += 1.0 / FPS;

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
	delete activeScene;
	delete cubeLight;
	delete whiteMaterial;
	
	return error;
}