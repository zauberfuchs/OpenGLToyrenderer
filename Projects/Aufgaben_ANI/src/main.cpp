#include <thread>
#include <chrono>
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
#include "Sphere.h"
#include "World.h"

#define FPS 30

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
	auto simpleMeshShader = new Shader("simpleMeshShader", "../Data/simpleMeshShader.vert", "../Data/simpleMeshShader.frag");
	auto simpleMaterialShader = new Shader("simpleMaterialShader", "../Data/simpleMaterialShader.vert", "../Data/simpleMaterialShader.frag");
	auto BRDFMaterialShader = new Shader("BRDFMaterialShader", "../Data/BRDFMaterialShader.vert", "../Data/BRDFMaterialShader.frag");
	auto PbrTextureShader = new Shader("PbrTextureShader", "../Data/PbrTextureShader.vert", "../Data/PbrTextureShader.frag");

	auto albedo = new Texture("../Data/Textures/Materials/rustediron/albedo.png", ETextureChannels::AlbedoMap);
	auto normal = new Texture("../Data/Textures/Materials/rustediron/normal.png", ETextureChannels::NormalMap);
	auto metallic = new Texture("../Data/Textures/Materials/rustediron/metallic.png", ETextureChannels::MetallicMap);
	auto roughness = new Texture("../Data/Textures/Materials/rustediron/roughness.png", ETextureChannels::RoughnessMap);
	auto ao = new Texture("../Data/Textures/Materials/rustediron/ao.png", ETextureChannels::AmbientOcclusionMap);

	World::Get().AddShader(lightShader);
	World::Get().AddShader(skyboxShader);
	World::Get().AddShader(simpleMaterialShader);
	World::Get().AddShader(simpleMeshShader);

	auto materialPBR = new MaterialPBR("PBR");
	materialPBR->SetColor(glm::vec3(0.3f, 0.3f, 0.3f));
	materialPBR->SetAlbedo(glm::vec3(0.5f, 0.0f, 0.0f));
	materialPBR->SetMetallic(1.0f);
	materialPBR->SetRoughness(0.05f);
	materialPBR->SetAo(1.0f);
	materialPBR->SetShader(BRDFMaterialShader);

	auto materialTexturePBR = new MaterialPBR("PBRTexture");
	materialTexturePBR->SetTexture(albedo);
	materialTexturePBR->SetTexture(normal);
	materialTexturePBR->SetTexture(metallic);
	materialTexturePBR->SetTexture(roughness);
	materialTexturePBR->SetTexture(ao);
	materialTexturePBR->SetShader(PbrTextureShader);

	auto whiteMaterial = new Material("white");
	whiteMaterial->Ambient = glm::vec3(0.3f, 0.3f, 0.3f);
	whiteMaterial->Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	whiteMaterial->Specular = glm::vec3(0.5f, 0.5f, 0.5f);
	whiteMaterial->Shininess = 32.0f;
	whiteMaterial->m_Color = glm::vec3(1.0f);
	whiteMaterial->SetShader(lightShader);

	auto greyMaterial = new Material("grey");
	greyMaterial->Ambient = glm::vec3(0.3f, 0.3f, 0.3f);
	greyMaterial->Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	greyMaterial->Specular = glm::vec3(0.5f, 0.5f, 0.5f);
	greyMaterial->Shininess = 32.0f;
	greyMaterial->m_Color = glm::vec3(0.5f, 0.5f, 0.5f);
	greyMaterial->SetShader(simpleMeshShader);
	greyMaterial->SetReflections(ReflectionType::Ambient);

	auto redMaterial = new Material("red");
	redMaterial->Ambient = glm::vec3(0.3f, 0.3f, 0.3f);
	redMaterial->Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	redMaterial->Specular = glm::vec3(0.5f, 0.5f, 0.5f);
	redMaterial->Shininess = 32.0f;
	redMaterial->m_Color = glm::vec3(0.8f, 0.2f, 0.2f);
	redMaterial->SetShader(simpleMeshShader);
	redMaterial->SetReflections(ReflectionType::Diffuse);

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
	SceneObject cube("cube");
	SceneObject lightCube("lightCube");
	SceneObject teapot("teapot");
	SceneObject dragon("dragon");
	SceneObject armadillo("armadillo");
	SceneObject bunny("bunny");
	SceneObject sphere("sphere");
	SceneObject sphere1("sphere1");

	Skybox skybox("universe");

	activeScene->AddRootChild(&ground);
	activeScene->AddRootChild(&lightCube);
	activeScene->AddRootChild(&cube);
	activeScene->AddRootChild(&teapot);
	activeScene->AddRootChild(&sphere);
	activeScene->AddRootChild(&sphere1);
	//activeScene->AddRootChild(&dragon);
	//activeScene->AddRootChild(&armadillo);
	//activeScene->AddRootChild(&bunny);

	ground.AddModel(new Model(std::string("ground")));
	teapot.AddModel(new Model("../Data/Models/test-models/teapot.obj"));
	//armadillo.AddModel(new Model("../Data/Models/test-models/armadillo.obj"));
	//dragon.AddModel(new Model("../Data/Models/test-models/dragon.obj"));
	//bunny.AddModel(new Model("../Data/Models/test-models/bunny.obj"));
	cube.AddModel(new Model(std::string("cube")));
	sphere.AddModel(new Model(std::string("sphere")));
	sphere1.AddModel(new Model(std::string("sphere1")));
	lightCube.AddModel(new Model(std::string("lightCube")));

	ground.GetModel().AddMesh(new Plane("ground"));
	cube.GetModel().AddMesh(new StudentCube("cube"));
	lightCube.GetModel().AddMesh(new StudentCube("lightCube"));
	sphere.GetModel().AddMesh(new Sphere("Sphere", 12));
	sphere1.GetModel().AddMesh(new Sphere("Sphere1", 12));
	lightCube.AddLight(cubeLight);
	activeScene->GetSceneObject("ground")->GetModel().GetMesh("ground")->SetMaterial(greyMaterial);
	activeScene->GetSceneObject("cube")->GetModel().GetMesh("cube")->SetMaterial(redMaterial);
	activeScene->GetSceneObject("lightCube")->GetModel().GetMesh("lightCube")->SetMaterial(whiteMaterial);
	activeScene->GetSceneObject("teapot")->GetModel().GetMesh(0)->SetMaterial(materialPBR);
	activeScene->GetSceneObject("sphere")->GetModel().GetMesh(0)->SetMaterial(materialTexturePBR);
	activeScene->GetSceneObject("sphere1")->GetModel().GetMesh(0)->SetMaterial(redMaterial);
	//activeScene->GetSceneObject("dragon")->GetModel().GetMesh(0)->SetMaterial(redMaterial);
	//activeScene->GetSceneObject("armadillo")->GetModel().GetMesh(0)->SetMaterial(redMaterial);
	//activeScene->GetSceneObject("bunny")->GetModel().GetMesh(0)->SetMaterial(redMaterial);
	
	lightCube.GetTransform()->Translate(cubeLight->Position, Space::Local);
	lightCube.GetTransform()->Scale(glm::vec3(0.2f), Space::Local);

	//armadillo.GetTransform()->Translate(glm::vec3(3.0f, 0.5f, 0.0f), Space::Local);
	teapot.GetTransform()->Translate(glm::vec3(-3.0f, 0.0f, 0.0f), Space::Local);
	//dragon.GetTransform()->Translate(glm::vec3(6.0f, 0.4f, 0.0f), Space::Local);
	//bunny.GetTransform()->Translate(glm::vec3(-6.0f, -0.3f, 0.0f), Space::Local);
	//armadillo.GetTransform()->Scale(glm::vec3(0.01f), Space::Local);
	//dragon.GetTransform()->Scale(glm::vec3(0.01f), Space::Local);
	//teapot.GetTransform()->Scale(glm::vec3(0.5f), Space::Local);
	//bunny.GetTransform()->Scale(glm::vec3(10.0f), Space::Local);

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
	delete lightShader;
	delete skyboxShader;
	delete activeScene;
	delete cubeLight;
	delete greyMaterial;
	delete redMaterial;
	delete whiteMaterial;
	
	return error;
}