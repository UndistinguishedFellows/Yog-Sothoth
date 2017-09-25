#include "UIConfiguration.h"
#include "../../../Application.h"

UIConfiguration::UIConfiguration()
{
	active = false;
	fpsLog.assign(100, 0.0f);
	msLog.assign(100, 0.0f);
}

UIConfiguration::~UIConfiguration()
{
}

void UIConfiguration::DoUpdate(float dt)
{
	for (uint i = 0; i < fpsLog.size() - 1; ++i)
	{
		fpsLog[i] = fpsLog[i + 1];
	}
	for (uint i = 0; i < msLog.size() - 1; ++i)
	{
		msLog[i] = msLog[i + 1];
	}
	fpsLog[fpsLog.size()-1] = 1.0 / dt;
	msLog[msLog.size() - 1] = dt * 1000;
}

void UIConfiguration::Draw()
{

	ImGuiWindowFlags testsWindowFlags = 0;

	ImGui::SetNextWindowSize(ImVec2(400, 700), ImGuiSetCond_FirstUseEver);
	if (!ImGui::Begin("Configuration", &active, testsWindowFlags))
	{
		ImGui::End();
		return;
	}
	ImGui::Spacing();

	if (ImGui::CollapsingHeader("Application"))
	{
		static char appName[128];
		strcpy_s(appName, 128, App->appName.c_str());
		if (ImGui::InputText("App Name", appName, 128))
		{

		}

		static char orgName[128];
		strcpy_s(orgName, 128, App->organization.c_str());
		if (ImGui::InputText("Organization", orgName, 128))
		{

		}
		
		char title[25];
		sprintf_s(title, 25, "Framerate %.1f", fpsLog[fpsLog.size()-1]);
		ImGui::PlotHistogram("##framerate", &fpsLog[0], fpsLog.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));

		sprintf_s(title, 25, "Miliseconds %.1f", msLog[msLog.size() - 1]);
		ImGui::PlotHistogram("##miliseconds", &msLog[0], msLog.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
	}

	if (ImGui::CollapsingHeader("Window"))
	{
		ImGui::Text("Window content");
		
		int resW, resH;
		resW = App->window->config.w_res;
		resH = App->window->config.h_res;
		if (ImGui::SliderInt("Width", &resW, 0, 1920))//TODO: Magic numbers, need to create max & min res at config
		{
			App->window->config.w_res = resW;
			App->window->setWidth(resW);
		}
		if (ImGui::SliderInt("Heigth", &resH, 0, 1080))//TODO: Magic numbers ^
		{
			App->window->config.h_res = resH;
			App->window->setHeigth(resH);
		}

		bool resizable = App->window->config.resizable;
		bool borderLess = App->window->config.borderless;
		bool fullScreen = App->window->config.fullscreen;
		bool fullDesk = App->window->config.fullscreenDesktop;

		if (ImGui::Checkbox("Resizable", &resizable))
		{
			App->window->config.resizable = resizable;
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Restart to apply");
		}ImGui::SameLine();

		if (ImGui::Checkbox("Borderless", &borderLess))
		{
			App->window->config.borderless = borderLess;
			App->window->setBorderless(!borderLess);
		}

		if (ImGui::Checkbox("Fullscreen", &fullScreen))
		{
			App->window->config.fullscreen = fullScreen;
			App->window->setFullScreen(fullScreen);
		}ImGui::SameLine();

		if (ImGui::Checkbox("Fullscreen Desktop", &fullDesk))
		{
			App->window->config.fullscreenDesktop = fullDesk;
			App->window->setFullScreenDesktop(fullDesk);
		}
	}

	if (ImGui::CollapsingHeader("Hardware"))
	{
		ImGui::Text("Hardware content");
	}

	ImGui::End();

}