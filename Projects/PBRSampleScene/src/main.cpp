#include "../../Engine/Utils/pch.h"

#include "../../Engine/Components/Model.h"
#include "../../Engine/Scene/StudentCube.h"
#include "../../Engine/Scene/Skybox.h"
#include "../../Engine/Scene/Sphere.h"
#include "../../Engine/Scene/Renderer.h"
#include "../../Engine/Window/Window.h"
#include "../../Engine/Window/ImGuiWindow.h"

#define FPS 30

int main() {

	auto window = new Window(1280, 720);
	World::Get().SetActiveWindow(window);
	int error = window->InitOpenGLContext();

	g_Camera = new Camera(glm::vec3(0.0f, 20.0f, 30.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -25.0);

	World::Get().SetActiveScene(new Scene("Main Scene"));

	Scene* activeScene = World::Get().GetActiveScene();
	activeScene->SetSceneCamera(g_Camera);

	srand(time(nullptr));

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
	const auto whiteMaterial = new Material("white");
	whiteMaterial->Ambient = glm::vec3(0.3f, 0.3f, 0.3f);
	whiteMaterial->Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	whiteMaterial->Specular = glm::vec3(0.5f, 0.5f, 0.5f);
	whiteMaterial->Shininess = 32.0f;
	whiteMaterial->m_Color = glm::vec3(1.0f);
	whiteMaterial->SetShader(World::Get().GetShader("lightShader"));

	const auto pointLight = new Light("pointLight");
	pointLight->SetType(LightSourceType::PointLight);
	pointLight->SetPosition(glm::vec3(10.2f, 4.0f, 2.0f));
	pointLight->SetConstant(1.0f);
	pointLight->SetLinear(0.9f);
	pointLight->SetQuadratic(0.032f);
	pointLight->SetColor(glm::vec3(1.0f));
	pointLight->CreatePointDepthMap(1024, 1024);

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

	ground.AddModel(new Model(std::string("ground")));
	ground.GetModel().AddMesh(new StudentCube("ground"));

	activeScene->AddRootChild(&ground);
	activeScene->GetSceneObject("ground")->GetModel().GetMesh(0)->SetMaterial(World::Get().GetMaterial("ceramictile"));

	ground.GetTransform()->Scale(glm::vec3(30.00f, .625f, 30.00f), Space::Local);
	ground.GetTransform()->Translate(glm::vec3(10.0f, 0.0f, 4.0f), Space::Local);


	///////////////////////////////////////////////////////////////////////////////
	// Light
	///////////////////////////////////////////////////////////////////////////////
	
	SceneObject lightSphere("lightSphere");

	lightSphere.AddModel(new Model(std::string("lightSphere")));
	lightSphere.GetModel().AddMesh(new Sphere("Sphere", 32));
	lightSphere.AddLight(pointLight);

	activeScene->AddRootChild(&lightSphere);
	activeScene->GetSceneObject("lightSphere")->GetModel().GetMesh(0)->SetMaterial(whiteMaterial);

	lightSphere.GetTransform()->Translate(pointLight->GetPosition(), Space::Local);
	lightSphere.GetTransform()->Scale(glm::vec3(0.2f), Space::Local);



	///////////////////////////////////////////////////////////////////////////////
	// Setup Scene
	///////////////////////////////////////////////////////////////////////////////

	activeScene->SetSceneSkybox(&skybox);
	activeScene->AddSceneLight(pointLight);

	///////////////////////////////////////////////////////////////////////////////
	// Init Window & Rendering
	///////////////////////////////////////////////////////////////////////////////

	//todo make static??
	//window->InitImGui();

	ImGuiWindow::Init();
	Renderer::Init();


	///////////////////////////////////////////////////////////////////////////////
	// main Rendering loop
	///////////////////////////////////////////////////////////////////////////////
	
	while (!glfwWindowShouldClose(window->m_Window)) {
		
		window->WindowRendering();
		//window->NewImGuiFrame();

		ImGuiWindow::NewFrame();

		activeScene->UpdateScene();

		Renderer::DepthPrePath();
		
		Renderer::GeometryPath();

		Renderer::SkyboxPath();

		Renderer::PostFxPath();
		

		//window->ImGuiRender();
		ImGuiWindow::Render();
		
		glfwSwapBuffers(window->m_Window);
		glfwPollEvents();
	}

	Renderer::Shutdown();

	// cleanup
	

	delete g_Camera;
	delete window;
	delete activeScene;
	delete pointLight;
	delete whiteMaterial;
	
	return error;
}