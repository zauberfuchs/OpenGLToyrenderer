#include <thread>
#include <chrono>
#include <filesystem>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include "Mesh.h"
#include "Plane.h"
#include "Aufgabe1/StudentCube.h"
#include "Skybox.h"
#include "SceneObject.h"
#include "Window.h"
#include "Framebuffer.h"
#include "Renderbuffer.h"
#include "Scene.h"
#include "Model.h"
#include "ReflectionProbe.h"
#include "Sphere.h"
#include "World.h"

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

	auto lightShader = new Shader("lightShader", "../Data/lightShader.vert", "../Data/lightShader.frag");
	auto skyboxShader = new Shader("skyboxShader", "../Data/skyboxShader.vert", "../Data/skyboxShader.frag");
	auto simpleMeshShader = new Shader("simpleMeshShader", "../Data/simpleMeshShader.vert", "../Data/simpleMeshShader.frag");
	auto simpleMaterialShader = new Shader("simpleMaterialShader", "../Data/simpleMaterialShader.vert", "../Data/simpleMaterialShader.frag");
	auto BRDFMaterialShader = new Shader("brdfMaterialShader", "../Data/BRDFMaterialShader.vert", "../Data/BRDFMaterialShader.frag");
	auto PbrTextureShader = new Shader("pbrTextureShader", "../Data/PbrTextureShader.vert", "../Data/PbrTextureShader.frag");
	auto IrradianceConvolutionShader = new Shader("irradianceConvolutionShader", "../Data/cubemap.vert", "../Data/irradianceConvolution.frag");
	auto BRDFShader = new Shader("brdfShader", "../Data/brdf.vert", "../Data/brdf.frag");
	auto PrefilterShader = new Shader("prefilterShader", "../Data/cubemap.vert", "../Data/prefilter.frag");
	auto EquirectangularToCubemap = new Shader("equirectangularToCubemap", "../Data/cubemap.vert", "../Data/equirectangularToCubemap.frag");

	//Todo World, load shader folder / add shaders 
	World::Get().AddShader(lightShader);
	World::Get().AddShader(skyboxShader);
	World::Get().AddShader(simpleMaterialShader);
	World::Get().AddShader(simpleMeshShader);
	World::Get().AddShader(IrradianceConvolutionShader);
	World::Get().AddShader(BRDFShader);
	World::Get().AddShader(PrefilterShader);
	World::Get().AddShader(EquirectangularToCubemap);

	Skybox skybox("universe");

	ReflectionProbe probeOne(512, 512);
	//probeOne.CreateReflectionMapFromHDR("../Data/Textures/Hdr/Newport_Loft_Ref.hdr");
	probeOne.SetReflectionMap(skybox.GetId());
	//skybox.SetId(probeOne.GetReflectionMap());
	probeOne.Create();
	
	auto materialPBR = new MaterialPBR("PBR");
	materialPBR->SetAlbedo(glm::vec3(0.5f, 0.0f, 0.0f));
	materialPBR->SetMetallic(1.0f);
	materialPBR->SetRoughness(0.05f);
	materialPBR->SetAo(1.0f);
	materialPBR->SetShader(BRDFMaterialShader);

	auto materialTexturePBR = new MaterialPBR("PBRTexture");
	//materialTexturePBR->SetPBRTexture("../Data/Textures/Materials/gold");
	materialTexturePBR->SetPBRTexture("../Data/Textures/Materials/mirror");
	materialTexturePBR->SetReflectionProbe(&probeOne);
	materialTexturePBR->SetShader(PbrTextureShader);
	//Todo: phongmaterial
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
	
	SceneObject cube("cube");
	SceneObject lightCube("lightCube");
	SceneObject teapot("teapot");
	SceneObject dragon("dragon");
	SceneObject armadillo("armadillo");
	SceneObject bunny("bunny");
	SceneObject sphere("sphere");
	SceneObject sphere1("sphere1");

	


	
	activeScene->AddRootChild(&lightCube);
	activeScene->AddRootChild(&cube);
	activeScene->AddRootChild(&teapot);
	activeScene->AddRootChild(&sphere);
	//activeScene->AddRootChild(&sphere1);
	//activeScene->AddRootChild(&dragon);
	//activeScene->AddRootChild(&armadillo);
	//activeScene->AddRootChild(&bunny);
	
	teapot.AddModel(new Model("../Data/Models/test-models/teapot.obj"));
	//armadillo.AddModel(new Model("../Data/Models/test-models/armadillo.obj"));
	//dragon.AddModel(new Model("../Data/Models/test-models/dragon.obj"));
	//bunny.AddModel(new Model("../Data/Models/test-models/bunny.obj"));
	cube.AddModel(new Model(std::string("cube")));
	sphere.AddModel(new Model(std::string("sphere")));
	sphere1.AddModel(new Model(std::string("sphere1")));
	lightCube.AddModel(new Model(std::string("lightCube")));
	
	cube.GetModel().AddMesh(new StudentCube("cube"));
	lightCube.GetModel().AddMesh(new StudentCube("lightCube"));
	sphere.GetModel().AddMesh(new Sphere("Sphere", 256));
	sphere1.GetModel().AddMesh(new Sphere("Sphere1", 2));
	lightCube.AddLight(cubeLight);
	activeScene->GetSceneObject("cube")->GetModel().GetMesh("cube")->SetMaterial(redMaterial);
	activeScene->GetSceneObject("cube")->GetModel().GetMesh("cube")->SetMaterial(materialTexturePBR);
	activeScene->GetSceneObject("lightCube")->GetModel().GetMesh("lightCube")->SetMaterial(whiteMaterial);
	activeScene->GetSceneObject("teapot")->GetModel().GetMesh(0)->SetMaterial(materialPBR);
	activeScene->GetSceneObject("sphere")->GetModel().GetMesh(0)->SetMaterial(materialTexturePBR);
	//activeScene->GetSceneObject("sphere1")->GetModel().GetMesh(0)->SetMaterial(redMaterial);
	//activeScene->GetSceneObject("dragon")->GetModel().GetMesh(0)->SetMaterial(redMaterial);
	//activeScene->GetSceneObject("armadillo")->GetModel().GetMesh(0)->SetMaterial(redMaterial);
	//activeScene->GetSceneObject("bunny")->GetModel().GetMesh(0)->SetMaterial(redMaterial);
	
	lightCube.GetTransform()->Translate(cubeLight->Position, Space::Local);
	lightCube.GetTransform()->Scale(glm::vec3(0.2f), Space::Local);
	cube.GetTransform()->Translate(glm::vec3(3.0f, 0.0f, 0.0f), Space::Local);

	//armadillo.GetTransform()->Translate(glm::vec3(3.0f, 0.5f, 0.0f), Space::Local);
	teapot.GetTransform()->Translate(glm::vec3(-3.0f, 0.0f, 0.0f), Space::Local);
	//dragon.GetTransform()->Translate(glm::vec3(6.0f, 0.4f, 0.0f), Space::Local);
	//bunny.GetTransform()->Translate(glm::vec3(-6.0f, -0.3f, 0.0f), Space::Local);
	//armadillo.GetTransform()->Scale(glm::vec3(0.01f), Space::Local);
	//dragon.GetTransform()->Scale(glm::vec3(0.01f), Space::Local);
	//teapot.GetTransform()->Scale(glm::vec3(0.5f), Space::Local);
	//bunny.GetTransform()->Scale(glm::vec3(10.0f), Space::Local);
	

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


