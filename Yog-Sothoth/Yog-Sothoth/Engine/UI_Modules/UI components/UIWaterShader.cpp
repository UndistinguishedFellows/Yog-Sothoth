#include "UIWaterShader.h"
#include "../../../imGUI/imgui.h"
#include "../../../Application.h"

void UIWaterShader::DoUpdate(float dt)
{
}

void UIWaterShader::Draw()
{
	ImGuiWindowFlags outilnerWindowFlags = 0;
	outilnerWindowFlags |= ImGuiWindowFlags_AlwaysHorizontalScrollbar;
	//outilnerWindowFlags |= ImGuiWindowFlags_NoMove;
	//outilnerWindowFlags |= ImGuiWindowFlags_NoResize;

	ImGui::SetNextWindowSize(ImVec2(App->window->screen_surface->w / 8 * 7, 600), ImGuiSetCond_FirstUseEver);
	//ImGui::SetNextWindowPos(ImVec2(App->window->screen_surface->w / 8 * 7, 20));
	if (!ImGui::Begin("Water Shader", &active, outilnerWindowFlags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	if (ImGui::DragFloat("Offset1", &App->renderer->offset1, 0.01f)){}
	if (ImGui::DragFloat("Hz1", &App->renderer->Hz1, 0.01f)) {}
	if (ImGui::DragFloat("A1", &App->renderer->A1, 0.01f)) {}
	if (ImGui::DragFloat("Offset2", &App->renderer->offset2, 0.01f)) {}
	if (ImGui::DragFloat("Hz2", &App->renderer->Hz2, 0.01f)) {}
	if (ImGui::DragFloat("A2", &App->renderer->A2, 0.01f)) {}

	ImGui::End();

	/////////////////////////////////////////////////////////////////////////////////////////////////


}
