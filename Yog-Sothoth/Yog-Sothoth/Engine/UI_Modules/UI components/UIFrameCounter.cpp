#include "UIFrameCounter.h"
#include <cmath>
#include "../../../Application.h"


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
	this->intFps = 1 / dt;
}

void UIFrameCounter::Draw()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10));
	ImGui::SetNextWindowSize(ImVec2(200, 200));
	if (!ImGui::Begin("", &active, ImVec2(0, 0), 0.3f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
	{
		ImGui::End();
		return;
	}
	ImGui::Text("MS: %.3f", dt);
	//ImGui::Separator();
//	ImGui::Text("FPS: %.2f", fps);
//	ImGui::Text("FPS: %d", intFps);
	ImGui::Text("FPS: %d", App->appTimer.GetLastFPS());
	ImGui::End();

}
