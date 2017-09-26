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
	//fpsLog[fpsLog.size()-1] = 1.0 / dt;
	fpsLog[fpsLog.size()-1] = App->appTimer.GetLastFPS();
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
		static float winBright;
		winBright = App->window->getBrightness();
		if (ImGui::SliderFloat("Brightness", &winBright, 0.0f, 1.0f))
		{
			//TODO: Add Brightness to config --> App->window->config.brightness =winBright;
			App->window->setBrightness(winBright);
		}

		static int resW, resH;
		resW = App->window->getWidth();
		resH = App->window->getHeigth();
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
			if (!fullScreen && !fullDesk)
			{
				App->window->config.borderless = borderLess;
				App->window->setBorderless(!borderLess);
			}
			else
			{
				borderLess = false;
			}
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Only if FullScreen modes are not enabled");
		}

		if (ImGui::Checkbox("Fullscreen", &fullScreen))
		{
			if (!fullDesk)
			{
				App->window->config.fullscreen = fullScreen;
				App->window->setFullScreen(fullScreen);
			}
			else
			{
				fullScreen = false;
			}
			
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Fullscreen Desktop must be off");
		}ImGui::SameLine();

		if (ImGui::Checkbox("Fullscreen Desktop", &fullDesk))
		{
			if (!fullScreen)
			{
				App->window->config.fullscreenDesktop = fullDesk;
				App->window->setFullScreenDesktop(fullDesk);
			}
			else
			{
				fullDesk = false;
			}
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Fullscreen must be off");
		}
	}
	if (ImGui::CollapsingHeader("Render"))
	{
		ImGui::Text("Render content");

		bool depthTest = App->renderer->GetDepthTest();
		bool cullFace = App->renderer->GetCullFace();
		bool wireframe = App->renderer->GetWireframe();

		if (ImGui::Checkbox("DepthTest", &depthTest))
		{
			App->renderer->SetDepthTest(depthTest);
		}ImGui::SameLine();
		if (ImGui::Checkbox("CullFace", &cullFace))
		{
			App->renderer->SetCullFace(cullFace);
		}
		if (ImGui::Checkbox("Wireframe", &wireframe))
		{
			App->renderer->SetWireframe(wireframe);
		}
		if (ImGui::Button("Save##render"))
		{
			App->renderer->SaveConfig();
		}

	}

	if (ImGui::CollapsingHeader("Hardware"))
	{
		ImGui::Text("SDL Version: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "%d.%d.%d", App->hardwareInfo.sdlVersion.major,App->hardwareInfo.sdlVersion.minor,App->hardwareInfo.sdlVersion.patch);

		ImGui::Text("Platform: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "%s", App->hardwareInfo.platform);

		ImGui::Separator();

		ImGui::Text("CPUs: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "%d (Cache: %dKB)", App->hardwareInfo.numCPUs, App->hardwareInfo.cache);
	
		ImGui::Text("System RAM: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "%d GB", App->hardwareInfo.systemRAM);

		ImGui::Text("Caps: "); ImGui::SameLine();
		if (App->hardwareInfo.DNow)
		{
			ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "3DNow,"); ImGui::SameLine();
		}
		if (App->hardwareInfo.AVX)
		{
			ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "AVX,"); ImGui::SameLine();
		}
		if (App->hardwareInfo.AVX2)
		{
			ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "AVX2,"); ImGui::SameLine();
		}
		if (App->hardwareInfo.altiVec)
		{
			ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "AltiVec,"); ImGui::SameLine();
		}
		if (App->hardwareInfo.MMX)
		{
			ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "MMX,"); ImGui::SameLine();
		}
		if (App->hardwareInfo.RDTSC)
		{
			ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "RDTSC,"); ImGui::SameLine();
		}
		if (App->hardwareInfo.SSE)
		{
			ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "SSE,"); ImGui::SameLine();
		}
		if (App->hardwareInfo.SSE2)
		{
			ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "SSE2,"); ImGui::SameLine();
		}
		if (App->hardwareInfo.SSE3)
		{
			ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "SSE3,"); ImGui::SameLine();
		}
		if (App->hardwareInfo.SSE41)
		{
			ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "SSE41,"); ImGui::SameLine();
		}
		if (App->hardwareInfo.SSE42)
		{
			ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "SSE42,"); ImGui::SameLine();
		}
		ImGui::Text(" ");
		ImGui::Separator();

		App->renderer->setGPUInfo();

		ImGui::Text("GPU: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "Vendor: %d Device: %d", App->renderer->gpuInfo.vendor,App->renderer->gpuInfo.deviceId);

		ImGui::Text("Brand: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "%s", App->renderer->gpuInfo.brand);

		ImGui::Text("VRAM budget: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "%.3f Mb", App->renderer->gpuInfo.videoMemBudget);
	
		ImGui::Text("VRAM usage: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "%.3f Mb", App->renderer->gpuInfo.videoMemUsage);

		ImGui::Text("VRAM avaliable: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "%.3f Mb", App->renderer->gpuInfo.videoMemAvaliable);

		ImGui::Text("VRAM reserved: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "%.3f Mb", App->renderer->gpuInfo.videoMemReserved);
	}

	ImGui::End();

}