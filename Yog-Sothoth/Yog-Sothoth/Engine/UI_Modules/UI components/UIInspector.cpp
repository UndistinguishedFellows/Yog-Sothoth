﻿#include "UIInspector.h"
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

		Transform();
		Mesh();
		Material();
		Camera();
	}

	ImGui::Spacing();



	ImGui::End();

	/////////////////////////////////////////////////////////////////////////////////////////////////


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
				if (!App->renderer->fbxViewer)
					transform->SetScale(scale);
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
			ImGui::Separator();
		}

}

void UIInspector::Mesh()
{
	C_Mesh* mesh = (C_Mesh*)App->objManager->GetFocusGO()->Mesh;
	if (mesh != nullptr)
		if (ImGui::CollapsingHeader("Mesh"))
		{
			ImGui::Text("Mesh");

			ImGui::Text("Triangle count: "); ImGui::SameLine();
			ImGui::Text("%d", mesh->indices.numIndices/3);

			ImGui::Text("Num Vertices: "); 
			ImGui::SameLine(); 
			ImGui::Text("%d", mesh->vertices.numVertices);
			ImGui::SameLine();
			ImGui::Text("Id Vertices: "); 
			ImGui::SameLine(); 
			ImGui::Text("%d", mesh->vertices.idVertices);

			ImGui::Text("Num Indices: "); 
			ImGui::SameLine(); 
			ImGui::Text("%d", mesh->indices.numIndices);
			ImGui::SameLine();
			ImGui::Text("Id Indices: "); 
			ImGui::SameLine(); 
			ImGui::Text("%d", mesh->indices.idIndices);

			ImGui::Text("Num Normals: ");
			ImGui::SameLine();
			ImGui::Text("%d", mesh->normals.numNormals);
			ImGui::SameLine();
			ImGui::Text("Id Normals: ");
			ImGui::SameLine();
			ImGui::Text("%d", mesh->normals.idNormals);

			ImGui::Text("Num UV: ");
			ImGui::SameLine();
			ImGui::Text("%d", mesh->vertices.numVertices);
			ImGui::SameLine();
			ImGui::Text("Id UV: ");
			ImGui::SameLine();
			ImGui::Text("%d", mesh->uv.idUV);

			ImGui::Separator();

			if (ImGui::Checkbox("Normals", &mesh->drawNormals)) {}
			ImGui::SameLine();
			if (ImGui::Checkbox("Wireframe##mesh", &mesh->wireframe)) {}

			ImGui::Separator();
		}
}

void UIInspector::Material()
{
	C_Material* material = (C_Material*)App->objManager->GetFocusGO()->Material;
	C_Mesh* mesh = (C_Mesh*)App->objManager->GetFocusGO()->Mesh;
	Color color;
	if (material != nullptr && mesh != nullptr)
		if (ImGui::CollapsingHeader("Material"))
		{
			ImGui::Text("Material");
			ImGui::Text("Name: %s", material->imInfo.name.c_str());
			ImGui::Text("Color");
			color = mesh->color;
			if (ImGui::DragFloat3("Color", &color, 0.01f, 0.f, 1.f))
			{
				mesh->color.Set(color.r, color.g, color.b, color.a);
			}
			if (material->texture != 0)
			{
				ImGui::Text("%dx%d", material->imInfo.width, material->imInfo.height);
				//ImGui::Text("%d bytes", material->imInfo.bytes); //Not working
				ImGui::Image((ImTextureID)material->texture, ImVec2(256, 256), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
			}

			if (ImGui::Checkbox("Checkers##mat", &material->checkers)) {}
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
	C_Camera* camera = App->objManager->GetFocusGO()->Camera;
	if (camera != nullptr)
		if (ImGui::CollapsingHeader("Camera##inspector"))
		{
			ImGui::Text("Camera");
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
