#include "Engine/Utils/pch.h"
#include "ImGuiWindow.h"


#include "Window.h"
#include "Engine/OpenGL/Texture.h"
#include "Engine/Scene/World.h"
#include "Engine/Scene/Renderer.h"
#include "Engine/Components/Animation.h"
#include "Engine/Components/Model.h"

void ImGuiWindow::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.Fonts->AddFontFromFileTTF("../Data/Fonts/OpenSans-Regular.ttf", 25);
	// Setup Dear ImGui style
	// ----------------------
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	// --------------------------------
	ImGui_ImplGlfw_InitForOpenGL(World::Get().GetActiveWindow()->m_Window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	s_ImGuiData.ButtonTextures.insert({ std::string("playBTN"), new Texture("../Data/Textures/PlayButton.png") });
	s_ImGuiData.ButtonTextures.insert({ std::string("stopBTN"), new Texture("../Data/Textures/StopButton.png") });
	s_ImGuiData.ButtonTextures.insert({ std::string("pauseBTN"), new Texture("../Data/Textures/PauseButton.png") });
}

void ImGuiWindow::AddUnderLine(ImColor color)
{
	ImVec2 min = ImGui::GetItemRectMin();
	ImVec2 max = ImGui::GetItemRectMax();
	min.y = max.y;
	ImGui::GetWindowDrawList()->AddLine(
		min, max, color, 1.0f);
}

void ImGuiWindow::RenderScenePanel()
{
	ImGui::Begin("Scene Panel");
	std::string name;
	static std::string current_item = "";
	auto sceneObjects = World::Get().GetActiveScene()->GetSceneObjects();

	ImGui::Text("Scene Objects", 20);
	AddUnderLine(ImColor(255, 255, 255, 255));
	ImGuiStyle& style = ImGui::GetStyle();
	float w = ImGui::CalcItemWidth();
	float spacing = style.ItemInnerSpacing.x;
	float button_sz = ImGui::GetFrameHeight();
	ImGui::PushItemWidth(w - spacing * 2.0f - button_sz * 2.0f);
	if (ImGui::BeginCombo("##Scene Objects", current_item.c_str()))
	{
		for (const auto [n, sceneObject] : sceneObjects)
		{
			name = sceneObject->GetName();
			bool is_selected = current_item == name.c_str();
			if (ImGui::Selectable(name.c_str(), is_selected))
			{
				current_item = name.c_str();
				s_ImGuiData.CurrentSceneObject = sceneObjects.at(current_item);
				s_ImGuiData.Transform = s_ImGuiData.CurrentSceneObject->GetTransform();
				s_ImGuiData.MaterialColor = s_ImGuiData.CurrentSceneObject->GetModel().GetMeshes().begin()->second->GetMaterial()->GetColor();

			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();
	ImGui::Dummy(ImVec2(20, 20));
	ImGui::SameLine(0, style.ItemInnerSpacing.x);

	if (current_item != "")
	{
		RenderTransformComponent();
		RenderMeshComponent();
		RenderLightComponent();
		RenderAnimationComponent();
	}
	RenderWireFrameMode();
	RenderMSAA();
	RenderFpsCounter();

	ImGui::End();
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiWindow::RenderTransformComponent()
{


	ImGui::Dummy(ImVec2(20, 20));
	ImGui::Text("Transform", 20);
	AddUnderLine(ImColor(255, 255, 255, 255));
	ImGui::Dummy(ImVec2(1, 1));
	DrawVec3Control("Translation", s_ImGuiData.Transform->GetLocalPosition());
	DrawVec3Control("Rotation", s_ImGuiData.Transform->GetLocalEulerAngles());
	DrawVec3Control("Scale", s_ImGuiData.Transform->GetLocalScale());
}

void ImGuiWindow::RenderMeshComponent()
{
	if (s_ImGuiData.CurrentSceneObject->GetModel().GetMeshes().size() != 0)
	{
		auto meshes = s_ImGuiData.CurrentSceneObject->GetModel().GetMeshes();

		ImGui::Dummy(ImVec2(20, 20));
		ImGui::Text("Material Component", 20);
		AddUnderLine(ImColor(255, 255, 255, 255));
		for(auto m : meshes)
		{
			auto material = (Material*)m.second->GetMaterial();
			s_ImGuiData.materials.insert(material);

			ImGui::Dummy(ImVec2(1, 1));
			ImGui::Text((std::string("Name: ") + material->GetName() + std::string(" Material")).c_str(), 50);
			ImGui::Dummy(ImVec2(1, 1));

			switch (material->GetType())
			{
			case MaterialType::Phong:
				ImGui::Dummy(ImVec2(10, 10));
				ImGui::Text(m.first.c_str(), 20);
				glm::vec3 color = material->GetColor();
				ImGui::ColorEdit3("Model Color", (float*)&color);
				material->SetColor(color);

				s_ImGuiData.Ambient = material->GetAmbient();
				s_ImGuiData.Diffuse = material->GetDiffuse();
				s_ImGuiData.Specular = material->GetSpecular();
				s_ImGuiData.Shininess = material->GetShininess();

				DrawVec3Control("Ambient", s_ImGuiData.Ambient, 0, 0.01f, 0, 100, true);
				DrawVec3Control("Diffuse", s_ImGuiData.Diffuse, 0, 0.01f, 0, 100, true);
				DrawVec3Control("Specular", s_ImGuiData.Specular, 0, 0.01f, 0, 100, true);
				ImGui::Dummy(ImVec2(1, 1));
				ImGui::DragFloat("Shininess", &s_ImGuiData.Shininess, 1, 1, 256);

				s_ImGuiData.HasAmbientReflection = material->GetHasAmbient();
				s_ImGuiData.HasDiffuseReflection = material->GetHasDiffuse();
				s_ImGuiData.HasSpecularReflection = material->GetHasSpecular();

				ImGui::Checkbox("Toggle Ambient Reflection", &s_ImGuiData.HasAmbientReflection);
				if (s_ImGuiData.HasAmbientReflection)
				{
					material->SetHasAmbient(true);
				}
				else
				{
					material->SetHasAmbient(false);
				}

				ImGui::Checkbox("Toggle Diffuse Reflection", &s_ImGuiData.HasDiffuseReflection);
				if (s_ImGuiData.HasDiffuseReflection)
				{
					material->SetHasDiffuse(true);
				}
				else
				{
					material->SetHasDiffuse(false);
				}

				ImGui::Checkbox("Toggle Specular Reflection", &s_ImGuiData.HasSpecularReflection);
				if (s_ImGuiData.HasSpecularReflection)
				{
					material->SetHasSpecular(true);
				}
				else
				{
					material->SetHasSpecular(false);
				}

				material->SetAmbient(s_ImGuiData.Ambient);
				material->SetDiffuse(s_ImGuiData.Diffuse);
				material->SetSpecular(s_ImGuiData.Specular);
				material->SetShininess(s_ImGuiData.Shininess);

				break;
			case MaterialType::PhysicallyBased:
				s_ImGuiData.Albedo = material->GetAlbedo();
				s_ImGuiData.Metallic = material->GetMetallic();
				s_ImGuiData.Roughness = material->GetRoughness();
				s_ImGuiData.AmbientOcclusion = material->GetAo();
				DrawVec3Control("Albedo", s_ImGuiData.Albedo, 0, 1, 0, 100, true);
				ImGui::Dummy(ImVec2(1, 1));
				ImGui::DragFloat("Metallic", &s_ImGuiData.Metallic, 0.01f, 0, 1);
				ImGui::DragFloat("Roughness", &s_ImGuiData.Roughness, 0.01f, 0.01f, 1);
				ImGui::DragFloat("Ambient Occlusion", &s_ImGuiData.AmbientOcclusion, 0.01f, 0, 1);

				material->SetAlbedo(s_ImGuiData.Albedo);
				material->SetMetallic(s_ImGuiData.Metallic);
				material->SetRoughness(s_ImGuiData.Roughness);
				material->SetAo(s_ImGuiData.AmbientOcclusion);
				break;
			case MaterialType::TexturedPhysicallyBased :
				if(ImGui::ImageButton((ImTextureID)material->GetTexture(TextureType::AlbedoMap)->GetTextureID(), { 100, 100 }, { 0, 1 }, { 1, 0 }))
				{
					s_ImGuiData.SelectedMesh = m.second;
					s_ImGuiData.MaterialWindowState = true;
				}
			}
		}
		if (s_ImGuiData.MaterialWindowState)
		{
			ImGui::Begin("Materials");
			int i = 0;
			for (const auto& m : World::Get().GetMaterials())
			{
				i++;

				if (ImGui::ImageButton((ImTextureID)m.second->GetTexture(TextureType::AlbedoMap)->GetTextureID(), { 100, 100 }, { 0, 1 }, { 1, 0 }))
				{
					s_ImGuiData.SelectedMesh->SetMaterial(m.second);
					s_ImGuiData.MaterialWindowState = false;
				}
				if (i % 5 != 0)
				{
					ImGui::SameLine();
				}
				
			}
			ImGui::End();
		}
		

	}

	for (auto [name, mesh] : s_ImGuiData.CurrentSceneObject->GetModel().GetMeshes())
	{
		mesh->GetMaterial()->GetColor() = s_ImGuiData.MaterialColor;
	}
}

void ImGuiWindow::RenderLightComponent()
{
	if (s_ImGuiData.CurrentSceneObject->GetLight() != nullptr)
	{
		Light* l = s_ImGuiData.CurrentSceneObject->GetLight();
		s_ImGuiData.LightColor = l->GetColor();
		ImGui::Dummy(ImVec2(20, 20));
		ImGui::Text("Light Component", 20);
		AddUnderLine(ImColor(255, 255, 255, 255));
		ImGui::ColorEdit3("Light Color", &s_ImGuiData.LightColor.x);
		l->SetColor(s_ImGuiData.LightColor);
		s_ImGuiData.CurrentSceneObject->GetLight()->SetPosition(s_ImGuiData.Transform->GetLocalPosition());
	}
}

void ImGuiWindow::RenderAnimationComponent()
{
	if (s_ImGuiData.CurrentSceneObject->GetAnimation() != nullptr)
	{
		Animation* a = s_ImGuiData.CurrentSceneObject->GetAnimation();
		ImGui::Dummy(ImVec2(20, 20));
		ImGui::Text("Animation Component", 20);
		AddUnderLine(ImColor(255, 255, 255, 255));
		ImGui::Dummy(ImVec2(1, 1));
		if (ImGui::ImageButton((ImTextureID)s_ImGuiData.ButtonTextures.at("playBTN")->GetTextureID(), { 25, 25 }, { 0, 1 }, { 1, 0 }))
		{
			a->StartAnimation();
		}
		ImGui::SameLine();
		if (ImGui::ImageButton((ImTextureID)s_ImGuiData.ButtonTextures.at("pauseBTN")->GetTextureID(), { 25, 25 }, { 0, 1 }, { 1, 0 }))
		{
			a->PauseAnimation();
		}
		ImGui::SameLine();
		if (ImGui::ImageButton((ImTextureID)s_ImGuiData.ButtonTextures.at("stopBTN")->GetTextureID(), { 25, 25 }, { 0, 1 }, { 1, 0 }))
		{
			a->ResetAnimation();
		}
		ImGui::Dummy(ImVec2(1, 1));
		if (ImGui::Button("Print TimeLine", { 135, 35 }))
		{
			a->PrintTimeline();
		}
	}
}

void ImGuiWindow::RenderWireFrameMode()
{
	ImGui::Dummy(ImVec2(20, 20));
	ImGui::Text("Renderer Settings", 20);
	AddUnderLine(ImColor(255, 255, 255, 255));
	ImGui::Dummy(ImVec2(10, 10));
	ImGui::Checkbox("Wireframe Mode", &s_ImGuiData.WireMode);
	if (s_ImGuiData.WireMode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	
}

void ImGuiWindow::RenderMSAA()
{
	ImGui::Dummy(ImVec2(20, 20));
	ImGui::RadioButton("no Antialiasing", s_Data.MSAA, 1);
	ImGui::RadioButton("4x MSAA", s_Data.MSAA, 4);
	ImGui::RadioButton("8x MSAA", s_Data.MSAA, 8);
	ImGui::RadioButton("16x MSAA", s_Data.MSAA, 16);
}

void ImGuiWindow::RenderFpsCounter()
{
	ImGui::Dummy(ImVec2(20, 20));
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}


void ImGuiWindow::DrawVec3Control(const std::string& label, glm::vec3& values, const float& vMin, const float& vMax, float resetValue, float columnWidth, bool rgbMode)
{
	ImGuiIO& io = ImGui::GetIO();

	auto boldFont = io.Fonts->Fonts[0];


	ImGui::PushID(label.c_str());
	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
	ImVec2 buttonSize = { 30.0f, 30.0f };

	ImGui::PushItemWidth(100);
	ImGui::PushItemWidth(ImGui::CalcItemWidth());
	ImGui::PushItemWidth(ImGui::CalcItemWidth());
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button(rgbMode ? "R" : "X", buttonSize))
		values.x = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##X", &values.x, 0.1f, vMin, vMax, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button(rgbMode ? "G" : "Y", buttonSize))
		values.y = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Y", &values.y, 0.1f, vMin, vMax, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button(rgbMode ? "B" : "Z", buttonSize))
		values.z = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Z", &values.z, 0.1f, vMin, vMax, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();
}

void ImGuiWindow::NewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiWindow::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
