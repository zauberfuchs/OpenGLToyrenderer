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
	auto shadowmapShader = new Shader("shadowMap", "../Data/shadowmapShader.vert", "../Data/shadowmapShader.frag");

	//Todo World, load shader folder / add shaders 
	World::Get().AddShader(lightShader);
	World::Get().AddShader(skyboxShader);
	World::Get().AddShader(simpleMaterialShader);
	World::Get().AddShader(simpleMeshShader);
	World::Get().AddShader(IrradianceConvolutionShader);
	World::Get().AddShader(BRDFShader);
	World::Get().AddShader(PrefilterShader);
	World::Get().AddShader(EquirectangularToCubemap);
	World::Get().AddShader(shadowmapShader);

	Skybox skybox("universe");

	ReflectionProbe probeOne(512, 512);
	probeOne.CreateReflectionMapFromHDR("../Data/Textures/Hdr/Newport_Loft_Ref.hdr");
	skybox.SetId(probeOne.GetReflectionMap());
	probeOne.Create();

	//TODO: Material set with load all material in a specific gold
	auto materialGold = new MaterialPBR("PBRTextureGold");
	materialGold->SetPBRTexture("../Data/Textures/Materials/gold");
	materialGold->SetReflectionProbe(&probeOne);
	materialGold->SetShader(PbrTextureShader);
	auto materialHardwood = new MaterialPBR("PBRTextureHardwood");
	materialHardwood->SetPBRTexture("../Data/Textures/Materials/hardwood");
	materialHardwood->SetReflectionProbe(&probeOne);
	materialHardwood->SetShader(PbrTextureShader);
	auto materialYogamat = new MaterialPBR("PBRTextureYogamat");
	materialYogamat->SetPBRTexture("../Data/Textures/Materials/yogamat");
	materialYogamat->SetReflectionProbe(&probeOne);
	materialYogamat->SetShader(PbrTextureShader);
	auto materialWalnut = new MaterialPBR("PBRTextureWalnut");
	materialWalnut->SetPBRTexture("../Data/Textures/Materials/walnut");
	materialWalnut->SetReflectionProbe(&probeOne);
	materialWalnut->SetShader(PbrTextureShader);
	auto materialRustedIron = new MaterialPBR("PBRTextureRustedIron");
	materialRustedIron->SetPBRTexture("../Data/Textures/Materials/rustediron");
	materialRustedIron->SetReflectionProbe(&probeOne);
	materialRustedIron->SetShader(PbrTextureShader);
	auto materialPine = new MaterialPBR("PBRTexturePine");
	materialPine->SetPBRTexture("../Data/Textures/Materials/pine");
	materialPine->SetReflectionProbe(&probeOne);
	materialPine->SetShader(PbrTextureShader);
	auto materialLeather = new MaterialPBR("PBRTextureLeather");
	materialLeather->SetPBRTexture("../Data/Textures/Materials/leather");
	materialLeather->SetReflectionProbe(&probeOne);
	materialLeather->SetShader(PbrTextureShader);
	auto materialHextech = new MaterialPBR("PBRTextureHextech");
	materialHextech->SetPBRTexture("../Data/Textures/Materials/hextech");
	materialHextech->SetReflectionProbe(&probeOne);
	materialHextech->SetShader(PbrTextureShader);
	auto materialGras = new MaterialPBR("PBRTextureGras");
	materialGras->SetPBRTexture("../Data/Textures/Materials/gras");
	materialGras->SetReflectionProbe(&probeOne);
	materialGras->SetShader(PbrTextureShader);
	auto materialGranite = new MaterialPBR("PBRTextureGranite");
	materialGranite->SetPBRTexture("../Data/Textures/Materials/granite");
	materialGranite->SetReflectionProbe(&probeOne);
	materialGranite->SetShader(PbrTextureShader);
	auto materialFoam = new MaterialPBR("PBRTextureFoam");
	materialFoam->SetPBRTexture("../Data/Textures/Materials/foam");
	materialFoam->SetReflectionProbe(&probeOne);
	materialFoam->SetShader(PbrTextureShader);
	auto materialCopper = new MaterialPBR("PBRTextureCopper");
	materialCopper->SetPBRTexture("../Data/Textures/Materials/copper");
	materialCopper->SetReflectionProbe(&probeOne);
	materialCopper->SetShader(PbrTextureShader);
	auto materialCeramicTile = new MaterialPBR("PBRTextureCeramicTile");
	materialCeramicTile->SetPBRTexture("../Data/Textures/Materials/ceramictile");
	materialCeramicTile->SetReflectionProbe(&probeOne);
	materialCeramicTile->SetShader(PbrTextureShader);
	auto materialCelticGold = new MaterialPBR("PBRTextureCelticGold");
	materialCelticGold->SetPBRTexture("../Data/Textures/Materials/celticgold");
	materialCelticGold->SetReflectionProbe(&probeOne);
	materialCelticGold->SetShader(PbrTextureShader);
	auto materialAluminium = new MaterialPBR("PBRTextureAluminium");
	materialAluminium->SetPBRTexture("../Data/Textures/Materials/aluminium");
	materialAluminium->SetReflectionProbe(&probeOne);
	materialAluminium->SetShader(PbrTextureShader);


	//Todo: phongmaterial
	auto whiteMaterial = new Material("white");
	whiteMaterial->Ambient = glm::vec3(0.3f, 0.3f, 0.3f);
	whiteMaterial->Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	whiteMaterial->Specular = glm::vec3(0.5f, 0.5f, 0.5f);
	whiteMaterial->Shininess = 32.0f;
	whiteMaterial->m_Color = glm::vec3(1.0f);
	whiteMaterial->SetShader(lightShader);

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
	
	activeScene->GetSceneObject("shaderball1")->GetModel().GetMesh(0)->SetMaterial(materialHardwood);
	activeScene->GetSceneObject("shaderball1")->GetModel().GetMesh(1)->SetMaterial(materialCeramicTile);

	activeScene->GetSceneObject("shaderball2")->GetModel().GetMesh(0)->SetMaterial(materialPine);
	activeScene->GetSceneObject("shaderball2")->GetModel().GetMesh(1)->SetMaterial(materialYogamat);

	activeScene->GetSceneObject("shaderball3")->GetModel().GetMesh(0)->SetMaterial(materialCelticGold);
	activeScene->GetSceneObject("shaderball3")->GetModel().GetMesh(1)->SetMaterial(materialFoam);

	activeScene->GetSceneObject("shaderball4")->GetModel().GetMesh(0)->SetMaterial(materialRustedIron);
	activeScene->GetSceneObject("shaderball4")->GetModel().GetMesh(1)->SetMaterial(materialHextech);

	activeScene->GetSceneObject("shaderball5")->GetModel().GetMesh(0)->SetMaterial(materialAluminium);
	activeScene->GetSceneObject("shaderball5")->GetModel().GetMesh(1)->SetMaterial(materialCelticGold);

	activeScene->GetSceneObject("shaderball6")->GetModel().GetMesh(0)->SetMaterial(materialCopper);
	activeScene->GetSceneObject("shaderball6")->GetModel().GetMesh(1)->SetMaterial(materialGras);

	activeScene->GetSceneObject("shaderball7")->GetModel().GetMesh(0)->SetMaterial(materialFoam);
	activeScene->GetSceneObject("shaderball7")->GetModel().GetMesh(1)->SetMaterial(materialCopper);

	activeScene->GetSceneObject("shaderball8")->GetModel().GetMesh(0)->SetMaterial(materialLeather);
	activeScene->GetSceneObject("shaderball8")->GetModel().GetMesh(1)->SetMaterial(materialGranite);

	activeScene->GetSceneObject("shaderball9")->GetModel().GetMesh(0)->SetMaterial(materialGold);
	activeScene->GetSceneObject("shaderball9")->GetModel().GetMesh(1)->SetMaterial(materialWalnut);



	///////////////////////////////////////////////////////////////////////////////
	// ground
	///////////////////////////////////////////////////////////////////////////////

	SceneObject ground("ground");
	activeScene->AddRootChild(&ground);
	ground.AddModel(new Model(std::string("ground")));
	ground.GetModel().AddMesh(new StudentCube("ground"));
	activeScene->GetSceneObject("ground")->GetModel().GetMesh(0)->SetMaterial(materialCeramicTile);
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
	delete simpleMeshShader;
	delete lightShader;
	delete skyboxShader;
	delete activeScene;
	delete cubeLight;
	delete whiteMaterial;
	
	return error;
}


