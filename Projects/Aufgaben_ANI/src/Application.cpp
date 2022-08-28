#include "Engine/Utils/pch.h"

#include "Engine/Components/Model.h"
#include "Engine/Components/Animation.h"
#include "Engine/Geometry/Plane.h"
#include "Engine/Geometry/StudentCube.h"
#include "Engine/Geometry/Sphere.h"
#include "Engine/Scene/Renderer.h"
#include "Engine/Window/Window.h"
#include "Engine/Window/ImGuiWindow.h"

#define FPS 30

int main() {

	const auto window = new Window(1600, 1200);
	World::Get().SetActiveWindow(window);
	const int error = window->InitOpenGLContext();

	g_Camera = new Camera(glm::vec3(10.0f, 4.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 180.0f, -25.0);

	Scene* activeScene = new Scene("Main Scene");
	World::Get().SetActiveScene(activeScene);


	///////////////////////////////////////////////////////////////////////////////
	// Setup Shader / Materials
	///////////////////////////////////////////////////////////////////////////////
	
	ShaderLoader::LoadShaderFolder("../Data/Shaders/");
	//MaterialLoader::LoadMaterialFolder("../Data/Textures/Materials/");

	const auto skyBoxTexture = new Texture(TextureTarget::TextureCubeMap);
	skyBoxTexture->LoadCubemap("../Data/Textures/Skybox/darkish");

	auto wheelTexture = new Texture("../Data/Textures/wheel_bump.png", TextureTarget::Texture2D);
	wheelTexture->SetTextureTarget(TextureTarget::Texture2D);
	wheelTexture->SetUniformLocation("material.albedoMap");

	auto basketballTexture = new Texture("../Data/Textures/basketball.png", TextureTarget::Texture2D);
	basketballTexture->SetTextureTarget(TextureTarget::Texture2D);
	basketballTexture->SetUniformLocation("material.albedoMap");

	const auto whiteMaterial = new Material("white");
	whiteMaterial->SetType(MaterialType::Phong);
	whiteMaterial->SetAmbient(glm::vec3(1.0f, 1.0f, 1.0f));
	whiteMaterial->SetDiffuse(glm::vec3(0.5f, 0.5f, 0.5f));
	whiteMaterial->SetSpecular(glm::vec3(0.5f, 0.5f, 0.5f));
	whiteMaterial->SetShininess(32.0f);
	whiteMaterial->SetReflections(ReflectionType::Phong);
	whiteMaterial->SetColor(glm::vec3(1.0f));

	const auto wheelMaterial = new Material("wheel");
	wheelMaterial->SetType(MaterialType::Phong);
	wheelMaterial->SetAmbient(glm::vec3(0.3f, 0.3f, 0.3f));
	wheelMaterial->SetDiffuse(glm::vec3(0.5f, 0.5f, 0.5f));
	wheelMaterial->SetSpecular(glm::vec3(0.5f, 0.5f, 0.5f));
	wheelMaterial->SetShininess(32.0f);
	wheelMaterial->SetColor(glm::vec3(1.0f));
	wheelMaterial->SetTexture(wheelTexture);
	wheelMaterial->SetReflections(ReflectionType::Phong);

	const auto basketballMaterial = new Material("basketball");
	basketballMaterial->SetType(MaterialType::Phong);
	basketballMaterial->SetAmbient(glm::vec3(0.3f, 0.3f, 0.3f));
	basketballMaterial->SetDiffuse(glm::vec3(0.5f, 0.5f, 0.5f));
	basketballMaterial->SetSpecular(glm::vec3(0.5f, 0.5f, 0.5f));
	basketballMaterial->SetShininess(32.0f);
	basketballMaterial->SetColor(glm::vec3(1.0f));
	basketballMaterial->SetTexture(basketballTexture);
	basketballMaterial->SetReflections(ReflectionType::Phong);

	const auto greyMaterial = new Material("grey");
	greyMaterial->SetType(MaterialType::Phong);
	greyMaterial->SetAmbient(glm::vec3(0.7f, 0.7f, 0.7f));
	greyMaterial->SetDiffuse(glm::vec3(0.5f, 0.5f, 0.5f));
	greyMaterial->SetSpecular(glm::vec3(0.5f, 0.5f, 0.5f));
	greyMaterial->SetShininess(32.0f);
	greyMaterial->SetReflections(ReflectionType::Phong);
	greyMaterial->SetColor(glm::vec3(0.3f));

	///////////////////////////////////////////////////////////////////////////////
	// Create geometry
	///////////////////////////////////////////////////////////////////////////////

	SceneObject plane("plane");
	plane.AddModel(new Model(std::string("plane")));
	plane.GetModel().AddMesh(new Plane("plane"));
	plane.GetModel().GetMesh(0)->SetMaterial(greyMaterial);
	plane.GetTransform()->Scale(glm::vec3(20.0f, 20.0f, 20.0f));

	SceneObject wheel("wheel");
	wheel.AddModel(new Model("../Data/Models/wheel/Autoreifen.obj"));
	wheel.GetModel().GetMesh(0)->SetMaterial(wheelMaterial);
	wheel.GetTransform()->Translate(glm::vec3(0.0f, 0.4f, 0.0f), Space::Local);

	SceneObject basketball("basketball");
	basketball.AddModel(new Model("../Data/Models/basketball/Basketball.obj"));
	basketball.GetModel().GetMesh(0)->SetMaterial(basketballMaterial);
	basketball.GetTransform()->Translate(glm::vec3(0.0f, 1.7f, 0.0f), Space::Local);
	basketball.GetTransform()->Scale(glm::vec3(0.125f), Space::Local);

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
	
	
	activeScene->AddRootChild(&wheel);
	activeScene->AddRootChild(&basketball);
	activeScene->AddRootChild(&plane);
	activeScene->AddRootChild(&lightSphere);

	activeScene->AddSceneLight(pointLight);
	activeScene->SetSceneCamera(g_Camera);
	activeScene->SetSceneSkybox(skyBoxTexture);

	///////////////////////////////////////////////////////////////////////////////
	// Animation 1
	///////////////////////////////////////////////////////////////////////////////

	// Variablen
	constexpr float pi = 3.14159;

	uint16_t dauerSekunden = 5;
	int16_t endPositionZ = -6;
	float durchmesser = 0.797;

	// Berechnung der Umdrehungen
	float rotationX = glm::degrees(endPositionZ / (pi * durchmesser) * 2 * pi);

	// Ein wenig Schreibarbeit sparen
	Animation anim("wheel", &wheel, FPS);

	
	//f(t) = -2t� + 3t�
	auto hermiteSplineFunc = [](const float& i, const float& frames) {
		return -2 * glm::pow(i / (frames - 1), 3) + 3 * glm::pow(i / (frames - 1), 2);
	};
	anim.SetInterpolationFunc(hermiteSplineFunc);

	// Keys setzten und Reifen animieren
	// Wir m�ssen hier kein Start Keyframe setzen, da unsere Animation automatisch bei frame 0 Startet, falls wir das �ndern m�chten, ist dies aber m�glich.
	Keyframe endKey;
	endKey.Frame = dauerSekunden * FPS;
	endKey.Position = glm::vec3(0.0f, 0.4f, endPositionZ);
	endKey.Rotation = glm::vec3(rotationX, 0.0f, 0.0f);

	// Keys der Animation hinzuf�gen
	anim.InsertKeyframe(&endKey);

	///////////////////////////////////////////////////////////////////////////////
	// Animation 2 //todo sofern nichts anderes gesetzt wurde �bernehme werte vom objekt
	///////////////////////////////////////////////////////////////////////////////

	// Variablen
	const float g = 9.81; // erdbeschleunigung in m / s ^ 2
	const float durchmesserBall = 0.25; // Durchmesser in m
	const float radius = durchmesserBall / 2;
	float reduzierungsHoehe = 0.77; // % * 100
	float geschwindigkeit = 0.8; // 80 cm pro sek
	float geschwindigkeitsabfall = 0.85; // % * 100

	float hoehe = 1.7; // Starthoehe in m
	float zMove = 0.0f;

	uint16_t boinks = 9;
	uint16_t frame = 0;
	
	Animation animBall("ball", &basketball, FPS);

	//f(t) = -2t^3 + 3t^2
	animBall.SetInterpolationFunc(hermiteSplineFunc);

	for (int i = 0; i < boinks; i++)
	{
		Keyframe* key = new Keyframe;
		float t = glm::sqrt(2 * (hoehe - radius) / g);
		float tfps = t * FPS;
		frame += tfps;
		zMove += -geschwindigkeit * t;
		key->Frame = frame;
		key->Position = glm::vec3(0.0f, radius, zMove);
		key->Scale = glm::vec3(glm::vec3(0.125f));
		key->Rotation = glm::vec3(glm::vec3(-frame * 5, 0.0f, 0.0f));
		animBall.InsertKeyframe(key);

		key = new Keyframe;
		geschwindigkeit = geschwindigkeit * geschwindigkeitsabfall;
		hoehe = hoehe * reduzierungsHoehe;

		t = glm::sqrt(2 * (hoehe - radius) / g);
		tfps = t * FPS;
		frame += tfps;

		zMove += -geschwindigkeit * t;
		key->Frame = frame;
		key->Position = glm::vec3(0.0f, hoehe, zMove);
		key->Scale = glm::vec3(glm::vec3(0.125f));
		key->Rotation = glm::vec3(glm::vec3(-frame * 5, 0.0f, 0.0f));
		animBall.InsertKeyframe(key);
	}


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

		window->FrameRateLimit(FPS);

		// Animation update, k�nnte die welt verwalten oder ein manager
		anim.Update();
		animBall.Update();

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


