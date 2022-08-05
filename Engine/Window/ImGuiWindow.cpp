#include "../../Engine/Utils/pch.h"
#include "ImGuiWindow.h"


#include "Window.h"
#include "../../Engine/OpenGL/Texture.h"
#include "../../Engine/Scene/Camera.h"
#include "../../Engine/Scene/World.h"
#include "../../Engine/Scene/Renderer.h"

void ImGuiWindow::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	// ----------------------
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	// --------------------------------
	ImGui_ImplGlfw_InitForOpenGL(World::Get().GetActiveWindow()->m_Window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	s_ImGuiData.ButtonTextures.insert({ std::string("playBTN"), new Texture("../Data/Textures/PlayButton.png", TextureType::AlbedoMap) });
	s_ImGuiData.ButtonTextures.insert({ std::string("stopBTN"), new Texture("../Data/Textures/StopButton.png", TextureType::AlbedoMap) });
	s_ImGuiData.ButtonTextures.insert({ std::string("pauseBTN"), new Texture("../Data/Textures/PauseButton.png", TextureType::AlbedoMap) });
}

void ImGuiWindow::AddUnderLine(ImColor color)
{
	ImVec2 min = ImGui::GetItemRectMin();
	ImVec2 max = ImGui::GetItemRectMax();
	min.y = max.y;
	ImGui::GetWindowDrawList()->AddLine(
		min, max, color, 1.0f);
}

void ImGuiWindow::Render()
{
	// TODO, komplett überarbeiten, Model Auswählen, meshes auswählen, Materialen Auswählen etc pp

	ImGui::Begin("Scene Panel");
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 translation(0, 0, 0);
	glm::vec3 rotation(0, 0, 0);
	glm::vec3 scale(0, 0, 0);
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
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();

	ImGui::SameLine(0, style.ItemInnerSpacing.x);



	if (current_item != "")
	{
		auto s = sceneObjects.at(current_item);
		ImGui::Dummy(ImVec2(20, 20));
		translation = s->GetTransform()->GetLocalPosition();
		rotation = s->GetTransform()->GetLocalEulerAngles();
		scale = s->GetTransform()->GetLocalScale();
		color = s->GetModel().GetMeshes().begin()->second->GetMaterial()->GetColor();
		ImGui::Text("Transform", 20);
		AddUnderLine(ImColor(255, 255, 255, 255));
		ImGui::Dummy(ImVec2(1, 1));
		DrawVec3Control("Translation", translation);
		DrawVec3Control("Rotation", rotation);
		DrawVec3Control("Scale", scale);

		//ImGui::Dummy(ImVec2(20, 20));
		//ImGui::Text("Model Component", 20);
		//AddUnderLine(ImColor(255, 255, 255, 255));
		//ImGui::Dummy(ImVec2(1, 1));
		//ImGui::ColorEdit3("Model Color", (float*)&color);

		if (s->GetLight() != nullptr)
		{
			Light* l = s->GetLight();
			lightColor = l->GetColor();
			ImGui::Dummy(ImVec2(20, 20));
			ImGui::Text("Light Component", 20);
			AddUnderLine(ImColor(255, 255, 255, 255));
			ImGui::ColorEdit3("Light Color", &lightColor.x);
			l->SetColor(lightColor);
			s->GetLight()->SetPosition(translation);
		}

		if (s->GetModel().GetMeshes().size() != 0)
		{
			auto meshes = s->GetModel().GetMeshes();
			auto material = (Material*)meshes.begin()->second->GetMaterial();
			ImGui::Dummy(ImVec2(20, 20));
			ImGui::Text("Material Component", 20);
			AddUnderLine(ImColor(255, 255, 255, 255));
			ImGui::Dummy(ImVec2(1, 1));
			glm::vec3 color = material->GetColor();
			ImGui::ColorEdit3("Model Color", (float*)&color);
			material->SetColor(color);
			ImGui::Dummy(ImVec2(1, 1));
			ImGui::Text((std::string("Name: ") + material->GetName() + std::string(" Material")).c_str(), 50);
			ImGui::Dummy(ImVec2(1, 1));
			DrawVec3Control("Ambient", material->Ambient, 0, 1, 0, 100, true);
			DrawVec3Control("Diffuse", material->Diffuse, 0, 1, 0, 100, true);
			DrawVec3Control("Specular", material->Specular, 0, 1, 0, 100, true);
			ImGui::Dummy(ImVec2(1, 1));
			ImGui::DragFloat("Shininess", &material->Shininess, 1, 1, 256);

			s_ImGuiData.HasAmbientReflection = material->m_HasAmbient;
			s_ImGuiData.HasDiffuseReflection = material->m_HasDiffuse;
			s_ImGuiData.HasSpecularReflection = material->m_HasSpecular;

			ImGui::Checkbox("Toggle Ambient Reflection", &s_ImGuiData.HasAmbientReflection);
			if (s_ImGuiData.HasAmbientReflection)
			{
				material->m_HasAmbient = 1;
			}
			else
			{
				material->m_HasAmbient = 0;
			}

			ImGui::Checkbox("Toggle Diffuse Reflection", &s_ImGuiData.HasDiffuseReflection);
			if (s_ImGuiData.HasDiffuseReflection)
			{
				material->m_HasDiffuse = 1;
			}
			else
			{
				material->m_HasDiffuse = 0;
			}

			ImGui::Checkbox("Toggle Specular Reflection", &s_ImGuiData.HasSpecularReflection);
			if (s_ImGuiData.HasSpecularReflection)
			{
				material->m_HasSpecular = 1;
			}
			else
			{
				material->m_HasSpecular = 0;
			}

		}

		for (auto [name, mesh] : s->GetModel().GetMeshes())
		{
			mesh->GetMaterial()->GetColor() = color;
		}

		s->GetTransform()->Translate(translation, Space::Local);
		s->GetTransform()->Scale(scale, Space::Local);
		s->GetTransform()->SetRotation(rotation, Space::Local);

		if (s->GetAnimation() != nullptr)
		{
			Animation* a = s->GetAnimation();
			ImGui::Dummy(ImVec2(20, 20));
			ImGui::Text("Animation Component", 20);
			AddUnderLine(ImColor(255, 255, 255, 255));
			ImGui::Dummy(ImVec2(1, 1));
			if (ImGui::ImageButton((ImTextureID)s_ImGuiData.ButtonTextures.at("playBTN")->m_ID, {25, 25}, {0, 1}, {1, 0}))
			{
				a->StartAnimation();
			}
			ImGui::SameLine();
			if (ImGui::ImageButton((ImTextureID)s_ImGuiData.ButtonTextures.at("pauseBTN")->m_ID, { 25, 25 }, { 0, 1 }, { 1, 0 }))
			{
				a->PauseAnimation();
			}
			ImGui::SameLine();
			if (ImGui::ImageButton((ImTextureID)s_ImGuiData.ButtonTextures.at("stopBTN")->m_ID, { 25, 25 }, { 0, 1 }, { 1, 0 }))
			{
				a->ResetAnimation();
			}
			ImGui::Dummy(ImVec2(1, 1));
			if (ImGui::Button("Print TimeLine", { 125, 25 }))
			{
				a->PrintTimeline();
			}
		}

		ImGui::RadioButton("no Antialiasing", s_Data.MSAA, 1);
		ImGui::RadioButton("4x MSAA", s_Data.MSAA, 4);
		ImGui::RadioButton("8x MSAA", s_Data.MSAA, 8);
		ImGui::RadioButton("16x MSAA", s_Data.MSAA, 16);
	}
	ImGui::Dummy(ImVec2(20, 20));
	ImGui::Checkbox("Wireframe Mode", &s_ImGuiData.WireMode);
	if (s_ImGuiData.WireMode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	ImGui::Dummy(ImVec2(20, 20));
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();


	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
	ImVec2 buttonSize = { 20.0f, 20.0f };

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
