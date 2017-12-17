#include "UIInspector.h"
#include "../../MathGeoLib/MathGeoLib.h"
#include "../../../Application.h"
#include "../../Engine/GameObjects/M_ObjectManager.h"
#include "../../GameObjects/Components/C_Transform.h"
#include "../../GameObjects/Components/C_Mesh.h"

UIInspector::UIInspector()
{
}

UIInspector::~UIInspector()
{
}

void UIInspector::Draw()
{
	////////////////////////////////////////////////////////////////////////////////////////////////

	ImGuiWindowFlags outilnerWindowFlags = 0;
	outilnerWindowFlags |= ImGuiWindowFlags_AlwaysHorizontalScrollbar;
	//outilnerWindowFlags |= ImGuiWindowFlags_NoMove;
	//outilnerWindowFlags |= ImGuiWindowFlags_NoResize;

	ImGui::SetNextWindowSize(ImVec2(App->window->screen_surface->w / 8 * 7, 600), ImGuiSetCond_FirstUseEver);
	//ImGui::SetNextWindowPos(ImVec2(App->window->screen_surface->w / 8 * 7, 20));
	if (!ImGui::Begin("Inspector", &active, outilnerWindowFlags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}
	if (App->objManager->GetFocusGO() != nullptr)
	{
		ImGui::Text("Name: "); ImGui::SameLine(); ImGui::Text("%s", App->objManager->GetFocusGO()->name.c_str());
		
		//ImGuiStyle style = ImGui::GetStyle();
		//ImVec4 color(0, 0, 0, 255);
		//ImGui::ColorEdit4("BGCOlor", (float*)&style.Colors[2], true);
		if (ImGui::Checkbox("AABB##gameObject", &App->objManager->GetFocusGO()->drawAABB)){}

		if (!App->objManager->GetFocusGO()->serializable)
		{
			ImGui::TextColored(ImVec4(0.8f, 0.0f, 0.0f, 1.0f), "Not serializable");
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("GameObject changes won't be saved");
			}
		}
		Transform();
		Mesh();
		Material();
		Camera();
		Shader();

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();

//		if (ImGui::Button("Add Component"))
//			ImGui::OpenPopup("AddComponent");
//		if (ImGui::BeginPopup("AddComponent"))
//		{
//			ShowAddComponentMenu();
//			ImGui::EndPopup();
//		}
	}

	ImGui::Spacing();



	ImGui::End();

	/////////////////////////////////////////////////////////////////////////////////////////////////


}

void UIInspector::ShowAddComponentMenu()
{
	ImGui::MenuItem("Component", NULL, false, false);
	ImGui::Separator();

	bool enabler;
	enabler = true;

	if (App->objManager->GetFocusGO()->Mesh != nullptr)
		enabler = false;
	if (ImGui::MenuItem("Mesh"))
	{
		App->objManager->CreateComponent(App->objManager->GetFocusGO(), C_MESH);
	}
	enabler = true;

	if (App->objManager->GetFocusGO()->Material != nullptr)
		enabler = false;
	if (ImGui::MenuItem("Material"))
	{
		App->objManager->CreateComponent(App->objManager->GetFocusGO(), C_MATERIAL);
	}
	enabler = true;

	if (App->objManager->GetFocusGO()->Camera != nullptr)
		enabler = false;
	if (ImGui::MenuItem("Camera", "", false, enabler))
	{
		App->objManager->CreateComponent(App->objManager->GetFocusGO(), C_CAMERA);
	}
	enabler = true;
}

void UIInspector::Transform()
{
	C_Transform* transform = App->objManager->GetFocusGO()->Transform;
	if (transform != nullptr)
		if (ImGui::CollapsingHeader("Transform"))
		{
			ImGui::Text("Local Transform");
			float3 position;
			float3 scale;
			Quat rot;
			transform->localTransform.Decompose(position, rot, scale);

			float3 localEulerAngles(transform->GetRotation());

			if (ImGui::DragFloat3("Position", position.ptr(), 0.01f))
			{
				if (!App->renderer->fbxViewer)
					transform->SetPosition(position);
			}

			if (ImGui::DragFloat3("Rotation", localEulerAngles.ptr(), 1.f))
			{
				if (!App->renderer->fbxViewer)
				transform->SetRotation(localEulerAngles.x, localEulerAngles.y, localEulerAngles.z);
			}

			if (ImGui::DragFloat3("Scale", scale.ptr(), 0.01f, 0.01f))
			{
//				if (!App->renderer->fbxViewer)
//					transform->SetScale(scale);
			}


			//////////////////////////////////////////////////////////////////////////////////////////////////////////////
			ImGui::Text("Global Transform");

			float3 Gposition;
			float3 Gscale;
			Quat Grot;

			transform->globalTransform.Decompose(Gposition, Grot, Gscale);

			float3 eulerAngles = Grot.ToEulerXYZ();
			ImGui::DragFloat3("GPosition", Gposition.ptr(), 0.01f);
			ImGui::DragFloat3("GRotation", eulerAngles.ptr(), 0.01f);
			ImGui::DragFloat3("GScale", Gscale.ptr(), 0.01f);

			//Grot = Quat::FromEulerXYZ(eulerAngles.x, eulerAngles.y, eulerAngles.z);
		}

}

void UIInspector::Mesh()
{
	static bool closableMesh = true;

	C_Mesh* mesh = (C_Mesh*)App->objManager->GetFocusGO()->Mesh;
	if (mesh != nullptr && closableMesh)
	{
		if (ImGui::CollapsingHeader("Mesh", &closableMesh))
		{
			/*if (ImGui::Button("Set Mesh"))
				ImGui::OpenPopup("SetMesh");
			if (ImGui::BeginPopup("SetMesh"))
			{
				ImGui::MenuItem("Meshes", NULL, false, false);
				ImGui::Separator();
				
				//Meshes List
				ImGui::MenuItem("Mesh 1");
				ImGui::MenuItem("Mesh 2");
				ImGui::MenuItem("Mesh 3");
				ImGui::MenuItem("Mesh 4");
				
				ImGui::EndPopup();
			}
			ImGui::Spacing();
			*/
			ImGui::Text("Triangle count: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "%d", mesh->rMesh->indices.numIndices / 3);

			ImGui::Text("Num Vertices: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "%d", mesh->rMesh->vertices.numVertices);
			ImGui::SameLine();
			ImGui::Text("Id Vertices: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "%d", mesh->rMesh->vertices.idVertices);

			ImGui::Text("Num Indices: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "%d", mesh->rMesh->indices.numIndices);
			ImGui::SameLine();
			ImGui::Text("Id Indices: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "%d", mesh->rMesh->indices.idIndices);

			ImGui::Text("Num Normals: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "%d", mesh->rMesh->normals.numNormals);
			ImGui::SameLine();
			ImGui::Text("Id Normals: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "%d", mesh->rMesh->normals.idNormals);

			ImGui::Text("Num UV: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "%d", mesh->rMesh->vertices.numVertices);
			ImGui::SameLine();
			ImGui::Text("Id UV: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "%d", mesh->rMesh->uv.idUV);

			ImGui::Separator();

			if (ImGui::Checkbox("Normals", &mesh->drawNormals)) {}
			ImGui::SameLine();
			if (ImGui::Checkbox("Wireframe##mesh", &mesh->wireframe)) {}
		}
	}
	else
	{
		if (!closableMesh) {
			//Delete component mesh

			closableMesh = true;
		}
	}
}

void UIInspector::Material()
{
	static bool closableMaterial = true;

	C_Material* material = (C_Material*)App->objManager->GetFocusGO()->Material;
	C_Mesh* mesh = (C_Mesh*)App->objManager->GetFocusGO()->Mesh;
	Color color;
	if (material != nullptr && mesh != nullptr && closableMaterial)
	{
		if (ImGui::CollapsingHeader("Material", &closableMaterial))
		{/*
			if (ImGui::Button("Set Material"))
				ImGui::OpenPopup("SetMaterial");
			if (ImGui::BeginPopup("SetMaterial"))
			{
				ImGui::MenuItem("Materials", NULL, false, false);
				ImGui::Separator();

				//Meshes List
				ImGui::MenuItem("Material 1");
				ImGui::MenuItem("Material 2");
				ImGui::MenuItem("Material 3");
				ImGui::MenuItem("Material 4");

				ImGui::EndPopup();
			}
			ImGui::Spacing();
			*/
			ImGui::Text("Name: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "%s", material->imInfo.name.c_str());

			color = mesh->color;
			if (ImGui::DragFloat3("Color", &color, 0.01f, 0.f, 1.f))
			{
				mesh->color.Set(color.r, color.g, color.b, color.a);
			}
			if (material->rMaterial->texture != 0)
			{
				ImGui::Text("%dx%d", material->imInfo.width, material->imInfo.height);
				//ImGui::Text("%d bytes", material->imInfo.bytes); //Not working
				glBindTexture(GL_TEXTURE_2D, material->rMaterial->texture);
				ImGui::Image((ImTextureID)material->rMaterial->texture, ImVec2(256, 256), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
			}

			if (ImGui::Checkbox("Checkers##mat", &material->checkers)) {}
		}
	}
	else
	{
		if (!closableMaterial) {
			//Delete component material

			closableMaterial = true;
		}
	}
}

void UIInspector::Light()
{
	if (ImGui::CollapsingHeader("Light"))
	{

	}

}

void UIInspector::Camera()
{
	static bool closableCamera = true;

	C_Camera* camera = App->objManager->GetFocusGO()->Camera;
	if (camera != nullptr && closableCamera)
	{
		if (ImGui::CollapsingHeader("Camera##inspector", &closableCamera))
		{
			if (App->objManager->GetFocusGO() == App->objManager->activeCamera)
			{
				ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "Active camera");
			}
			else
			{
				if (ImGui::Button("Set active##camera"))
				{
					App->objManager->activeCamera->serializable = true;
					App->objManager->GetFocusGO()->serializable = false;
					App->objManager->activeCamera = App->objManager->GetFocusGO();
				}
			}
			if (App->objManager->GetFocusGO() == App->objManager->cullingCamera)
			{
				ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.0f, 1.0f), "Culling Camera");
			}
			else
			{
				if (ImGui::Button("Culling Camera##camera"))
				{
					App->objManager->cullingCamera->serializable = true;
					App->objManager->GetFocusGO()->serializable = false;
					App->objManager->cullingCamera = App->objManager->GetFocusGO();
				}
			}

			if (ImGui::DragFloat3("Position##camera", camera->frustum.pos.ptr(), 0.01f))
			{}
			if (ImGui::DragFloat3("UP##camera", camera->frustum.up.ptr(), 0.01f))
			{}
			if (ImGui::DragFloat3("Front##camera", camera->frustum.front.ptr(), 0.01f))
			{}
			if (ImGui::DragFloat("Horizontal FOV##camera", &camera->frustum.horizontalFov, 0.01f, 0, math::pi))
			{}
			if (ImGui::DragFloat("Vertical FOV##camera", &camera->frustum.verticalFov, 0.01f, 0, math::pi))
			{}
			if (ImGui::DragFloat("Near plane distance##camera", &camera->frustum.nearPlaneDistance, 0.01f, 0))
			{}
			if (ImGui::DragFloat("Far plane distance##camera", &camera->frustum.farPlaneDistance, 0.01f, camera->frustum.nearPlaneDistance))
			{}
			if (ImGui::DragFloat3("Custom orbit point##camera", camera->customOrbitPoint.ptr(), 0.01f))
			{}
			ImGui::Text("Aspect Ratio: %f", camera->frustum.AspectRatio());
		}
	}
	else
	{
		if (!closableCamera) {
			//Delete component camera

			closableCamera = true;
		}
	}
}

void UIInspector::Shader()
{
	static bool closableShader = true;
	if (ImGui::CollapsingHeader("Shader##inspector", &closableShader))
	{
		if (ImGui::Button("Reload Shaders")) App->resourceManager->reloadShaders = true;
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnDoubleClick;
		flags |= ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_DefaultOpen;
		flags |= ImGuiTreeNodeFlags_Selected;
		ImGuiTreeNodeFlags nodeFlags = 0;
		nodeFlags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
		nodeFlags |= ImGuiTreeNodeFlags_OpenOnArrow;
		nodeFlags |= ImGuiTreeNodeFlags_Leaf;
		//nodeFlags |= ImGuiTreeNodeFlags_Selected;

		std::vector<std::string> shaderNames;
		for (auto shader : App->resourceManager->shaders)
		{
			shaderNames.push_back(shader.first);
		}

		if (ImGui::TreeNodeEx("", flags))
		{
			for (auto item : shaderNames)
			{
				if (App->objManager->GetFocusGO()->shader.first.compare(item) == 0)
				{
					nodeFlags |= ImGuiTreeNodeFlags_Selected;
				}
				else
				{
					nodeFlags &= ~ImGuiTreeNodeFlags_Selected;
				}
				ImGui::TreeNodeEx(item.c_str(), nodeFlags);
				if (ImGui::IsItemClicked(0))
				{
					App->objManager->GetFocusGO()->shader = App->resourceManager->GetShader(item);
				}
				ImGui::TreePop();
			}

			ImGui::TreePop();
		}

		
	}
}

