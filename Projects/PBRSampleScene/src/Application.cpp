#include "Engine/Utils/pch.h"

#include "Engine/Components/Model.h"
#include "Engine/Geometry/StudentCube.h"
#include "Engine/Geometry/Sphere.h"
#include "Engine/Scene/Renderer.h"
#include "Engine/Window/Window.h"
#include "Engine/Window/ImGuiWindow.h"

#define FPS 30

int main() {
	const auto window = CreateRef<Window>(1600, 1200);
	World::Get().SetActiveWindow(window);
	const int error = window->InitOpenGLContext();

	g_Camera = CreateRef<Camera>(glm::vec3(0.0f, 20.0f, 30.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -25.0);

	Ref<Scene> activeScene = CreateRef<Scene>("Main Scene");
	World::Get().SetActiveScene(activeScene);

	srand(time(nullptr));

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
	whiteMaterial->SetColor(glm::vec3(1.0f));

	const auto pbrMaterial = CreateRef<Material>("pbr");
	pbrMaterial->SetType(MaterialType::PhysicallyBased);
	pbrMaterial->SetAlbedo(glm::vec3(1.0f, 0.0f, 0.0f));
	pbrMaterial->SetMetallic(0.140f);
	pbrMaterial->SetRoughness(0.2f);
	pbrMaterial->SetAo(0.250f);

	///////////////////////////////////////////////////////////////////////////////
	// Create geometry
	///////////////////////////////////////////////////////////////////////////////

	const unsigned int maxRows = 3;
	const unsigned int maxColumns = 3;
	
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
			shaderBalls[row][col]->AddModel(CreateRef<Model>("../Data/Models/test-models/ShaderballUnreal.obj"));
			shaderBalls[row][col]->GetTransform()->Translate(glm::vec3(col * 5.0f, 0.0f, row * 5.0f), Space::Local);
			shaderBalls[row][col]->GetTransform()->Scale(glm::vec3(0.01f, 0.01f, 0.01f), Space::Local);
		}
	}
	
	for(const auto& s : activeScene->GetSceneObjects())
	{
		if (s.first.find("shaderball") == 0)
		{
			s.second->GetModel()->GetMesh(0)->SetMaterial(World::Get().GetMaterial(rand() % 17));
			s.second->GetModel()->GetMesh(1)->SetMaterial(World::Get().GetMaterial(rand() % 17));
		}
	}

	auto teapot = CreateRef<SceneObject>("teapot");
	teapot->AddModel(CreateRef<Model>("../Data/Models/test-models/teapot.obj"));
	teapot->GetModel()->GetMesh()->SetMaterial(pbrMaterial);

	///////////////////////////////////////////////////////////////////////////////
	// ground
	///////////////////////////////////////////////////////////////////////////////

	auto ground = CreateRef<SceneObject>("ground");

	ground->AddModel(CreateRef<Model>(std::string("ground")));
	ground->GetModel()->AddMesh(CreateRef<StudentCube>("ground"));

	ground->GetModel()->GetMesh()->SetMaterial(World::Get().GetMaterial("ceramictile"));

	ground->GetTransform()->Scale(glm::vec3(30.00f, .625f, 30.00f), Space::Local);
	ground->GetTransform()->Translate(glm::vec3(10.0f, 0.0f, 4.0f), Space::Local);


	///////////////////////////////////////////////////////////////////////////////
	// Light
	///////////////////////////////////////////////////////////////////////////////

	const auto pointLight = CreateRef<Light>("pointLight");
	pointLight->SetType(LightSourceType::PointLight);
	pointLight->SetPosition(glm::vec3(10.2f, 4.0f, 2.0f));
	pointLight->SetConstant(1.0f);
	pointLight->SetLinear(0.9f);
	pointLight->SetQuadratic(0.032f);
	pointLight->SetColor(glm::vec3(1.0f));
	pointLight->CreatePointDepthMap(1024, 1024);


	auto lightSphere = CreateRef<SceneObject>("lightSphere");

	lightSphere->AddModel(CreateRef<Model>(std::string("lightSphere")));
	lightSphere->GetModel()->AddMesh(CreateRef<Sphere>("Sphere", 32));
	lightSphere->AddLight(pointLight);

	lightSphere->GetModel()->GetMesh()->SetMaterial(whiteMaterial);

	lightSphere->GetTransform()->Translate(pointLight->GetPosition(), Space::Local);
	lightSphere->GetTransform()->Scale(glm::vec3(0.2f), Space::Local);



	///////////////////////////////////////////////////////////////////////////////
	// Setup Scene
	///////////////////////////////////////////////////////////////////////////////

	Ref<ReflectionProbe> probeOne = CreateRef<ReflectionProbe>(1024, 1024);
	probeOne->CreateReflectionMapFromHDR("../Data/Textures/Hdr/Newport_Loft_Ref.hdr");
	//probeOne.SetReflectionMap(*skyBoxTexture);
	probeOne->Create();


	activeScene->AddRootChild(ground);
	activeScene->AddRootChild(lightSphere);
	//activeScene->AddRootChild(teapot);

	activeScene->SetReflectionProbe(probeOne);
	activeScene->SetSceneCamera(g_Camera);
	activeScene->SetSceneSkybox(probeOne->GetReflectionTexture());
	activeScene->AddSceneLight(pointLight);

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