#include "UIInspector.h"
#include "../../MathGeoLib/MathGeoLib.h"
#include "../../../Application.h"
#include "../../Engine/GameObjects/M_ObjectManager.h"
#include "../../GameObjects/Components/C_Transform.h"

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

		Transform();
		Mesh();
//		material();
//		camera();
	}

	ImGui::Spacing();



	ImGui::End();

	/////////////////////////////////////////////////////////////////////////////////////////////////


}

void UIInspector::Transform()
{
	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGui::Text("Local Transform");
		float3 position;
		float3 scale;
		Quat rot;
		C_Transform* transform = (C_Transform*)App->objManager->GetFocusGO()->FindComponent(C_TRANSFORM);
		if (transform != nullptr)
		{
			transform->localTransform.Decompose(position, rot, scale);

			float3 localEulerAngles(transform->GetRotation());

			if (ImGui::DragFloat3("Position", position.ptr(), 0.01f))
				transform->SetPosition(position);


			if (ImGui::DragFloat3("Rotation", localEulerAngles.ptr(), 1.f))
			{
				//localEulerAngles *= DEGTORAD;
				//rot = Quat::FromEulerXYZ(localEulerAngles.x, localEulerAngles.y, localEulerAngles.z);
				transform->SetRotation(localEulerAngles.x, localEulerAngles.y, localEulerAngles.z);
			}

			if (ImGui::DragFloat3("Scale", scale.ptr(), 0.01f))
				transform->SetScale(scale);


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

}

void UIInspector::Mesh()
{
	if (ImGui::CollapsingHeader("Mesh"))
	{
		ImGui::Text("Mesh");
		C_Mesh* mesh = (C_Mesh*)App->objManager->GetFocusGO()->FindComponent(C_MESH);
		if (mesh != nullptr)
		{
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
			ImGui::Text("%d", mesh->uv.numUV);
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
}

void UIInspector::Material()
{
	if (ImGui::CollapsingHeader("Material"))
	{
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
}
