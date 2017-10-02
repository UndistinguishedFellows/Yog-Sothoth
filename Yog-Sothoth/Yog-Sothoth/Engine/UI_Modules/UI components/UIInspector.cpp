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
//		mesh();
//		material();
//		camera();
	}

	ImGui::Spacing();



	ImGui::End();

	/////////////////////////////////////////////////////////////////////////////////////////////////


}

void UIInspector::Transform()
{
	ImGui::Text("Local Transform");
	float3 position;
	float3 scale;
	Quat rot;
	C_Transform* transform = (C_Transform*)App->objManager->GetFocusGO()->FindComponent(TRANSFORM);
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

void UIInspector::Mesh()
{
}

void UIInspector::Material()
{
}

void UIInspector::Camera()
{
}
