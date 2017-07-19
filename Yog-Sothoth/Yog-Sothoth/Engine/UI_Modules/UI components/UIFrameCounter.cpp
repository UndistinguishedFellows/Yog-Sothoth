#include "UIFrameCounter.h"



UIFrameCounter::UIFrameCounter()
{
}


UIFrameCounter::~UIFrameCounter()
{
}

void UIFrameCounter::DoUpdate(float dt)
{
	this->dt = dt;
	this->fps = 1.0 / dt;
}

void UIFrameCounter::Draw()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10));
	if (!ImGui::Begin("Example: Fixed Overlay", &pOpen, ImVec2(0, 0), 0.3f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
	{
		ImGui::End();
		return;
	}
	ImGui::Text("Simple overlay\non the top-left side of the screen.");
	ImGui::Separator();
	ImGui::Text("Mouse Position: (%.3f,%.0f)", dt, fps);
	ImGui::End();

}
