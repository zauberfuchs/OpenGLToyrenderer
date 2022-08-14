#include "Engine/Utils/pch.h"

#include "Engine/Components/Model.h"
#include "Engine/Components/Animation.h"
#include "Engine/Geometry/StudentCube.h"
#include "Engine/Geometry/Sphere.h"
#include "Engine/Scene/Skybox.h"
#include "Engine/Scene/Renderer.h"
#include "Engine/Window/Window.h"
#include "Engine/Window/ImGuiWindow.h"

#define FPS 30

int main() {

	const auto window = new Window(1600, 1200);
	World::Get().SetActiveWindow(window);
	const int error = window->InitOpenGLContext();

	g_Camera = new Camera(glm::vec3(0.0f, 20.0f, 30.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -25.0);

	Scene* activeScene = new Scene("Main Scene");
	World::Get().SetActiveScene(activeScene);


	///////////////////////////////////////////////////////////////////////////////
	// Setup Shader / Materials
	///////////////////////////////////////////////////////////////////////////////

	ShaderLoader::LoadShaderFolder("../Data/Shaders/");

	auto wheelTexture = new Texture("../Data/Textures/wheel_bump.png", TextureType::AlbedoMap);

	const auto whiteMaterial = new Material("white");
	whiteMaterial->SetType(MaterialType::Phong);
	whiteMaterial->SetAmbient(glm::vec3(1.0f, 1.0f, 1.0f));
	whiteMaterial->SetDiffuse(glm::vec3(0.5f, 0.5f, 0.5f));
	whiteMaterial->SetSpecular(glm::vec3(0.5f, 0.5f, 0.5f));
	whiteMaterial->SetShininess(32.0f);
	whiteMaterial->SetReflections(ReflectionType::Phong);
	whiteMaterial->SetColor(glm::vec3(1.0f));

	auto textureMaterial = new Material("grid");
	textureMaterial->SetType(MaterialType::Phong);
	textureMaterial->SetAmbient(glm::vec3(0.3f, 0.3f, 0.3f));
	textureMaterial->SetDiffuse(glm::vec3(0.5f, 0.5f, 0.5f));
	textureMaterial->SetSpecular(glm::vec3(0.5f, 0.5f, 0.5f));
	textureMaterial->SetShininess(32.0f);
	textureMaterial->SetColor(glm::vec3(1.0f));
	textureMaterial->SetTexture(wheelTexture);
	textureMaterial->SetReflections(ReflectionType::Phong);


	///////////////////////////////////////////////////////////////////////////////
	// Create geometry
	///////////////////////////////////////////////////////////////////////////////
	
	SceneObject cube("cube");
	cube.AddModel(new Model(std::string("cube")));
	cube.GetModel().AddMesh(new StudentCube("cube"));
	cube.GetModel().GetMesh("cube")->SetMaterial(whiteMaterial);


	SceneObject wheel("wheel");
	wheel.AddModel(new Model("../Data/Models/wheel/Autoreifen.obj"));
	wheel.GetModel().GetMesh(0)->SetMaterial(textureMaterial);
	wheel.GetTransform()->Translate(glm::vec3(2.0f));

	///////////////////////////////////////////////////////////////////////////////
	// Light
	///////////////////////////////////////////////////////////////////////////////

	const auto pointLight = new Light("pointLight");
	pointLight->SetType(LightSourceType::PointLight);
	pointLight->SetPosition(glm::vec3(10.2f, 4.0f, 2.0f));
	pointLight->SetConstant(1.0f);
	pointLight->SetLinear(0.9f);
	pointLight->SetQuadratic(0.032f);
	pointLight->SetColor(glm::vec3(1.0f));
	pointLight->CreatePointDepthMap(1024, 1024);

	SceneObject lightSphere("lightSphere");
	
	
	lightSphere.AddModel(new Model(std::string("lightSphere")));
	lightSphere.GetModel().AddMesh(new Sphere("Sphere", 32));
	lightSphere.AddLight(pointLight);

	lightSphere.GetModel().GetMesh(0)->SetMaterial(whiteMaterial);

	lightSphere.GetTransform()->Translate(pointLight->GetPosition(), Space::Local);
	lightSphere.GetTransform()->Scale(glm::vec3(0.2f), Space::Local);

	///////////////////////////////////////////////////////////////////////////////
	// Setup Scene
	///////////////////////////////////////////////////////////////////////////////

	Skybox skybox("universe");
	
	activeScene->AddRootChild(&cube);
	activeScene->AddRootChild(&wheel);
	activeScene->AddRootChild(&lightSphere);

	activeScene->SetSceneCamera(g_Camera);
	activeScene->SetSceneSkybox(&skybox);
	activeScene->AddSceneLight(pointLight);

	///////////////////////////////////////////////////////////////////////////////
	// Animation
	///////////////////////////////////////////////////////////////////////////////

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

		// Animation update, könnte die welt verwalten oder ein manager
		anim.Update();

		Renderer::DepthPrePath();

		Renderer::GeometryPath();

		Renderer::SkyboxPath();

		Renderer::PostFxPath();

		ImGuiWindow::RenderScenePanel();

		glfwSwapBuffers(window->m_Window);
		glfwPollEvents();
	}

	Renderer::Shutdown();

	delete g_Camera;
	delete window;
	delete activeScene;
	delete whiteMaterial;
	
	return error;
}


