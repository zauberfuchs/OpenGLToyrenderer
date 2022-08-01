#include "../../Engine/Utils/pch.h"

#include "../../Engine/Components/Model.h"
#include "../../Engine/Scene/StudentCube.h"
#include "../../Engine/Scene/Skybox.h"
#include "../../Engine/Scene/Sphere.h"
#include "../../Engine/Scene/Renderer.h"
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

	auto wheelTexture = new Texture("../Data/Textures/wheel_bump.png", ETextureChannels::AlbedoMap);

	//Todo: phongmaterial
	auto whiteMaterial = new Material("white");
	whiteMaterial->Ambient = glm::vec3(0.3f, 0.3f, 0.3f);
	whiteMaterial->Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	whiteMaterial->Specular = glm::vec3(0.5f, 0.5f, 0.5f);
	whiteMaterial->Shininess = 32.0f;
	whiteMaterial->m_Color = glm::vec3(1.0f);
	whiteMaterial->SetShader(World::Get().GetShader("lightShader"));

	auto textureMaterial = new Material("grid");
	textureMaterial->Ambient = glm::vec3(0.3f, 0.3f, 0.3f);
	textureMaterial->Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	textureMaterial->Specular = glm::vec3(0.5f, 0.5f, 0.5f);
	textureMaterial->Shininess = 32.0f;
	textureMaterial->m_Color = glm::vec3(1.0f);
	textureMaterial->SetShader(World::Get().GetShader("simpleMeshShader"));
	textureMaterial->SetTexture(wheelTexture);
	textureMaterial->SetReflections(ReflectionType::Phong);


	auto redMaterial = new Material("red");
	redMaterial->Ambient = glm::vec3(0.3f, 0.3f, 0.3f);
	redMaterial->Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	redMaterial->Specular = glm::vec3(0.5f, 0.5f, 0.5f);
	redMaterial->Shininess = 32.0f;
	redMaterial->m_Color = glm::vec3(0.8f, 0.2f, 0.2f);
	redMaterial->SetShader(World::Get().GetShader("simpleMeshShader"));
	redMaterial->SetReflections(ReflectionType::Diffuse);

	auto cubeLight = new Light("cubeLight");
	cubeLight->SetType(LightSourceType::PointLight);
	cubeLight->SetPosition(glm::vec3(10.2f, 4.0f, 2.0f));
	cubeLight->SetConstant(1.0f);
	cubeLight->SetLinear(0.9f);
	cubeLight->SetQuadratic(0.032f);
	cubeLight->SetColor(glm::vec3(1.0f));

	///////////////////////////////////////////////////////////////////////////////
	// Create geometry
	///////////////////////////////////////////////////////////////////////////////
	
	SceneObject cube("cube");
	
	activeScene->AddRootChild(&cube);
	
	cube.AddModel(new Model(std::string("cube")));
	
	cube.GetModel().AddMesh(new StudentCube("cube"));
	activeScene->GetSceneObject("cube")->GetModel().GetMesh("cube")->SetMaterial(redMaterial);

	/*SceneObject wheel("wheel");

	activeScene->AddRootChild(&wheel);

	wheel.AddModel(new Model("../Data/Models/wheel/Autoreifen.obj"));
	
	activeScene->GetSceneObject("wheel")->GetModel().GetMesh(0)->SetMaterial(textureMaterial);*/

	

	///////////////////////////////////////////////////////////////////////////////
	// Light
	///////////////////////////////////////////////////////////////////////////////

	SceneObject lightCube("lightCube");
	activeScene->AddRootChild(&lightCube);
	lightCube.AddModel(new Model(std::string("lightCube")));
	lightCube.GetModel().AddMesh(new Sphere("Sphere", 32));
	lightCube.AddLight(cubeLight);
	activeScene->GetSceneObject("lightCube")->GetModel().GetMesh(0)->SetMaterial(whiteMaterial);

	lightCube.GetTransform()->Translate(cubeLight->GetPosition(), Space::Local);
	lightCube.GetTransform()->Scale(glm::vec3(0.2f), Space::Local);
	

	///////////////////////////////////////////////////////////////////////////////
	// Setup Scene
	///////////////////////////////////////////////////////////////////////////////

	window->InitImGui();

	activeScene->SetSceneSkybox(&skybox);
	activeScene->AddSceneLight(cubeLight);

	Light* l = World::Get().GetActiveScene()->GetSceneLightSources().begin()->second;
	l->CreateDirectionalDepthMap(1024, 1024);

	///////////////////////////////////////////////////////////////////////////////
	// Animation
	///////////////////////////////////////////////////////////////////////////////

	double lasttime = glfwGetTime();

	// Erstlle Animation, für ein bestimmtes Sceneobjekt, und übergebe Fps dieser Animation
	Animation anim("Cube", &cube, FPS);

	Keyframe first;

	Keyframe second;
	second.Frame = FPS * 5;
	second.Position = glm::vec3(5.0, 5.0f, 0.0f);
	second.Scale = glm::vec3(3.0, 3.0f, 3.0f);
	second.Rotation = glm::vec3(0.0, 900.0f, 0.0f);

	Keyframe third;
	third.Frame = FPS * 10;
	third.Position = glm::vec3(5.0, 10.0f, 0.0f);
	third.Scale = glm::vec3(3.0, 3.0f, 3.0f);
	third.Rotation = glm::vec3(0.0, 900.0f, 900.0f);

	// füge keyframes zur Animation hinzu
	anim.InsertKeyframe(&first);
	anim.InsertKeyframe(&second);
	anim.InsertKeyframe(&third);

	Renderer::Init();

	///////////////////////////////////////////////////////////////////////////////
	// main Rendering loop
	///////////////////////////////////////////////////////////////////////////////

	while (!glfwWindowShouldClose(window->m_Window)) {
		window->WindowRendering();
		window->NewImGuiFrame();

		// kann man geschickter machen, man schläft direkt die richtige zeit nur einmal
		while (glfwGetTime() < lasttime + 1.0 / FPS) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		lasttime += 1.0 / FPS;

		// Animation update, könnte die welt verwalten oder ein manager
		anim.Update();
		Renderer::DepthPrePath();
		Renderer::GeometryPath();
		Renderer::SkyboxPath();

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
	delete redMaterial;
	delete whiteMaterial;
	
	return error;
}


