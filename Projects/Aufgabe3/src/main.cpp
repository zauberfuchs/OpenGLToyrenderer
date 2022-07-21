#include <chrono>
#include <thread>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Mesh.h"
#include "Plane.h"
#include "Skybox.h"
#include "SceneObject.h"
#include "Model.h"
#include "Window.h"
#include "Scene.h"
#include "World.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Studenten Anfang - Aufgabe 4, Teil5
/////////////////////////////////////////////////////////////////////////////////////////////////////////

// Input:  Orbit Rotation - Zeit in Tagen
// Rückgabe eines Winkels in Grad - Umlauf/Spin in Tagen
float GetOrbitRotation(float orbitTime)
{
	float result = -1;

	return result;
}


// Input:  Spin Rotation - Zeit in Stunden
// Rückgabe eines Winkels in Grad - Umlauf/Spin in Tagen
float GetSpinRotation(float spinTime)
{
	float result = -1;

	return result;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Studenten Ende
/////////////////////////////////////////////////////////////////////////////////////////////////////////



int main() {

	auto window = new Window(1250, 680);
	int error = window->InitOpenGLContext();

	World::Get().SetActiveScene(new Scene("Main Scene"));
	
	Scene* activeScene = World::Get().GetActiveScene();

	///////////////////////////////////////////////////////////////////////////////
	// Setup Shader / Materials / Lights
	///////////////////////////////////////////////////////////////////////////////
	
	auto skyboxShader = new Shader("skyboxShader", "../Data/skyboxShader.vert", "../Data/skyboxShader.frag");
	auto simpleMaterialShader = new Shader("simpleMaterialShader", "../Data/simpleMaterialShader.vert", "../Data/simpleMaterialShader.frag");

	World::Get().AddShader(simpleMaterialShader);
	World::Get().AddShader(skyboxShader);

	auto sunLight = new Light("sunLight");
	sunLight->Type = LightSourceType::PointLight;
	sunLight->Position = glm::vec3(0.0f, 8.0f, 0.0f);
	sunLight->Constant = 1.0f;
	sunLight->Linear = 0.001f;
	sunLight->Quadratic = 0.0001f;

	auto globalLight = new Light("globalLight");
	globalLight->Type = LightSourceType::DirectionalLight;
	globalLight->Direction = glm::vec3(0.0f, -1.0f, 0.0f);

	///////////////////////////////////////////////////////////////////////////////
	// Setup Scene
	///////////////////////////////////////////////////////////////////////////////
	
	SceneObject sun("sun");
	
	sun.AddModel(new Model("../Data/Models/sun/sun.obj"));
	
	Skybox skybox("universe");

	// Sonne Erbt von ihrem Elternteil die Translation
	sun.GetTransform()->SetConstraints({ Inheritance::Translation });
	
	activeScene->AddRootChild(&sun);

	//sun.AddChildren(&mercury);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Studenten Anfang - Aufgabe 4, Teil1
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Build SceneObject Hierarchie
	

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Studenten Anfang - Aufgabe 4, Teil2
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// Translationen der Orbit Umlaufbahnen
		
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Studenten Ende
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	

	
	///////////////////////////////////////////////////////////////////////////////
	// Setup Scene
	///////////////////////////////////////////////////////////////////////////////

	window->InitImGui();

	activeScene->SetSceneSkybox(&skybox);
	activeScene->SetSceneCamera(g_Camera);
	activeScene->AddSceneLight(sunLight);
	activeScene->AddSceneLight(globalLight);

	struct PlanetaryRotations
	{
		// Skaliere für Rotionen
		float Slider_SpinScaler = 1.0f;
		float Slider_OrbitScaler = 1.0f;
		float Slider_MoonsOrbitScaler = 1.0f;
		// Werte für Rotation in Tagen
		float SpinScaler = 1000.f * (0.0016f / 60.0f /* min */ / 60.0f /* std */ / 24.0f /* tage */);
		float OrbitScaler = 1000.0f * (0.0016 / 60.0f /* min */ / 60.0f /* std */ / 24.0f /* tage */);
		float MoonsOrbitScaler = 1000.0f * (0.0016f / 60.0f /* min */ / 60.0f /* std */ / 24.0f /* tage */);

		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Studenten Anfang alles was hier *Scaler steht. - Aufgabe 4, Teil3
		/////////////////////////////////////////////////////////////////////////////////////////////////////////

		float SunSpin = - GetSpinRotation(20.0f); // dies ist nur ein Beispiel Wert!!
		
		// Setzen der Orbits und Spin Rotationen mithilfe der Funktionen
		
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Studenten Ende
		/////////////////////////////////////////////////////////////////////////////////////////////////////////

	} planetaryRotations;



	///////////////////////////////////////////////////////////////////////////////
	// main Rendering loop
	///////////////////////////////////////////////////////////////////////////////
	
	while (!glfwWindowShouldClose(window->m_Window)) {
		

		window->NewImGuiFrame();

		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Studenten Anfang- Aufgabe 4, Teil4
		/////////////////////////////////////////////////////////////////////////////////////////////////////////

		// ...
		
		{
			// Beispiel Aufruf!
			//earth.GetTransform()->Rotate(glm::vec3(0.0f, planetaryRotations.Slider_SpinScaler* planetaryRotations.SpinScaler * planetaryRotations.EarthSpin, 0.0f), Space::Local);
			//earth.GetTransform()->Rotate(glm::vec3(0.0f, planetaryRotations.Slider_OrbitScaler * planetaryRotations.OrbitScaler * planetaryRotations.EarthOrbit, 0.0f), Space::World);
		}
		
		// ...
		
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Studenten Ende
		/////////////////////////////////////////////////////////////////////////////////////////////////////////

		window->WindowRendering();
		activeScene->UpdateScene();
		activeScene->RenderScene();

		ImGui::Begin("Planetary Control");		
		ImGui::SliderFloat("Spin Scaler", &planetaryRotations.Slider_SpinScaler, 1, 100000);		
		ImGui::Text("SpinScaler %f", planetaryRotations.SpinScaler);

		ImGui::SliderFloat("Orbit Scaler", &planetaryRotations.Slider_OrbitScaler, 1, 100000);	
		ImGui::Text("OrbitScaler %f", planetaryRotations.OrbitScaler);

		ImGui::SliderFloat("Moons Orbit Scaler", &planetaryRotations.Slider_MoonsOrbitScaler, 1, 1000);		
		ImGui::Text("MoonOrbitScaler %f", planetaryRotations.MoonsOrbitScaler);
		ImGui::End();

		window->ImGuiRender();

		glfwSwapBuffers(window->m_Window);
		glfwPollEvents();
	}

	// cleanup
	// destroy window and release resources
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	delete g_Camera;
	delete window;
	delete skyboxShader;
	delete simpleMaterialShader;
	delete activeScene;
	delete sunLight;
	delete globalLight;
	return error;
}