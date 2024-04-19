#include "Engine/Utils/pch.h"

#include "Engine/Components/Model.h"
#include "Engine/Geometry/Cube.h"
#include "Engine/Geometry/Sphere.h"
#include "Engine/Scene/Renderer.h"
#include "Engine/Window/Window.h"
#include "Engine/Window/ImGuiWindow.h"
#include "Engine/ECS/EntityManager.h"
#include "Engine/ECS/Entity.h"

#define FPS 30

int main() {
	const auto window = CreateRef<Window>(1600, 1200);
	World::Get().SetActiveWindow(window);
	const int error = window->InitOpenGLContext();

	g_Camera = CreateRef<Camera>(glm::vec3(0.0f, 20.0f, 30.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -25.0f);

	Ref<Scene> activeScene = CreateRef<Scene>("Main Scene");
	World::Get().SetActiveScene(activeScene);
	
	//ToDo asset handler der nur materialien lädt wenn sie genutzt werden

	///////////////////////////////////////////////////////////////////////////////
	// Setup Shader / Materials
	///////////////////////////////////////////////////////////////////////////////
	
	ShaderLoader::LoadShaderFolder("../Data/Shaders/");
	MaterialLoader::LoadMaterialFolder("../Data/Textures/Materials/");

	const auto skyBoxTexture = CreateRef<Texture>(TextureTarget::TextureCubeMap);
	skyBoxTexture->LoadCubemap("../Data/Textures/Skybox/darkish");
	
	const auto whiteMaterial = CreateRef<Material>("white");
	whiteMaterial->SetType(MaterialType::Phong);
	whiteMaterial->SetAmbient(glm::vec3(1.0f, 1.0f, 1.0f));
	whiteMaterial->SetDiffuse(glm::vec3(0.5f, 0.5f, 0.5f));
	whiteMaterial->SetSpecular(glm::vec3(0.5f, 0.5f, 0.5f));
	whiteMaterial->SetShininess(32.0f);
	whiteMaterial->SetReflections(ReflectionType::Phong);
	whiteMaterial->SetColor(glm::vec3(1.0f));\

	///////////////////////////////////////////////////////////////////////////////
	// Create geometry
	///////////////////////////////////////////////////////////////////////////////

	const unsigned int maxRows = 3;
	const unsigned int maxColumns = 3;

	//TODO AddComponent<Mesh> im model laden / hinzufügen zusammen mit der Entity. ?? kann man mehrere MeshComponents haben?
	
	Ref<SceneObject> shaderBalls[maxRows][maxColumns];

	int i = 1;
	for (int row = 0; row < maxRows; row++)
	{
		for (int col = 0; col < maxColumns; col++)
		{
			shaderBalls[row][col] = CreateRef<SceneObject>("shaderball" + std::to_string(i));
			i++;
		}
	}

	for (int row = 0; row < maxRows; row++)
	{
		for (int col = 0; col < maxColumns; col++)
		{
			activeScene->AddRootChild(shaderBalls[row][col]);
			shaderBalls[row][col]->AddComponent<Model>("../Data/Models/test-models/ShaderballUnreal.obj");
			shaderBalls[row][col]->GetComponent<Transform>().Translate(glm::vec3(col * 5.0f, 0.0f, row * 5.0f), Space::Local);
			shaderBalls[row][col]->GetComponent<Transform>().Scale(glm::vec3(0.01f, 0.01f, 0.01f), Space::Local);
		}
	}
	
	for(const auto& s : activeScene->GetSceneObjects())
	{
		if (s.first.find("shaderball") == 0)
		{
			s.second->GetComponent<Model>().GetMesh(0)->SetMaterial(World::Get().GetMaterial(rand() % 17));
			s.second->GetComponent<Model>().GetMesh(1)->SetMaterial(World::Get().GetMaterial(rand() % 17));
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// ground
	///////////////////////////////////////////////////////////////////////////////

	auto ground = CreateRef<SceneObject>("ground");

	ground->AddComponent<Model>(std::string("ground"));
	ground->GetComponent<Model>().AddMesh(CreateRef<Cube>("ground"));

	ground->GetComponent<Model>().GetMesh()->SetMaterial(World::Get().GetMaterial("ceramictile"));

	ground->GetComponent<Transform>().Scale(glm::vec3(30.00f, .625f, 30.00f), Space::Local);
	ground->GetComponent<Transform>().Translate(glm::vec3(10.0f, 0.0f, 4.0f), Space::Local);


	///////////////////////////////////////////////////////////////////////////////
	// Light
	///////////////////////////////////////////////////////////////////////////////
	
	auto lightSphere = CreateRef<SceneObject>("lightSphere");

	lightSphere->AddComponent<Model>(std::string("lightSphere"));
	lightSphere->GetComponent<Model>().AddMesh(CreateRef<Sphere>("Sphere", 32));
	lightSphere->GetComponent<Model>().GetMesh()->SetMaterial(whiteMaterial);

	lightSphere->GetComponent<Transform>().Translate({ 10.2f, 4.0f, 2.0f }, Space::Local);
	lightSphere->GetComponent<Transform>().Scale(glm::vec3(0.2f), Space::Local);

	lightSphere->AddComponent<Light>("pointLight");
	
	Light& pointLight = lightSphere->GetComponent<Light>();
	pointLight.SetType(LightSourceType::PointLight);
	//pointLight.SetPosition(glm::vec3(10.2f, 4.0f, 2.0f));
	pointLight.SetConstant(1.0f);
	pointLight.SetLinear(0.9f);
	pointLight.SetQuadratic(0.032f);
	pointLight.SetColor(glm::vec3(1.0f));
	pointLight.CreatePointDepthMap(1024, 1024);

	///////////////////////////////////////////////////////////////////////////////
	// Setup Scene
	///////////////////////////////////////////////////////////////////////////////

	Ref<ReflectionProbe> probeOne = CreateRef<ReflectionProbe>(1024, 1024);
	probeOne->CreateReflectionMapFromHDR("../Data/Textures/Hdr/Newport_Loft_Ref.hdr");
	//probeOne.SetReflectionMap(*skyBoxTexture);
	probeOne->Create();

	activeScene->AddRootChild(ground);
	activeScene->AddRootChild(lightSphere);

	activeScene->SetReflectionProbe(probeOne);
	activeScene->SetSceneCamera(g_Camera);
	activeScene->SetSceneSkybox(probeOne->GetReflectionTexture());

	///////////////////////////////////////////////////////////////////////////////
	// Init Window & Rendering
	///////////////////////////////////////////////////////////////////////////////

	ImGuiWindow::Init();
	Renderer::Init();

	///////////////////////////////////////////////////////////////////////////////
	// main Rendering loop
	///////////////////////////////////////////////////////////////////////////////
	
	while (!glfwWindowShouldClose(window->m_Window)) {
		
		window->WindowRendering();

		ImGuiWindow::NewFrame();

		Renderer::DepthPrePath();
		
		Renderer::GeometryPath();

		Renderer::SkyboxPath();

		Renderer::PostFxPath();

		ImGuiWindow::RenderScenePanel();
		
		glfwSwapBuffers(window->m_Window);
		glfwPollEvents();
	}

	Renderer::Shutdown();
	
	return error;
}