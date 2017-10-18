#include "M_UIManager.h"
#include "../../Globals.h"
#include "../../Application.h"
#include "../../imGUI/imgui.h"
#include "../../imGUI/imgui_impl_sdl_gl3.h"
//#include "../../OpenGL.h"

#include <SDL.h>

#include "UI components\UIConsole.h"


M_UIManager::M_UIManager(bool enabled) : Module(enabled)
{
	name.assign("uiManager");
}

M_UIManager::~M_UIManager()
{
}

bool M_UIManager::Init()
{
	bool ret = true;

	SDL_Log("Init editor gui with imgui lib version %s", ImGui::GetVersion());

	ImGui_ImplSdlGL3_Init(App->window->GetWindow());

	setGreyStyle();

	console = new UIConsole();
	console->active = true;
	frameCounter = new UIFrameCounter();
	frameCounter->active = true;
	outliner = new UIOutliner();
	tests = new UITests();
	configuration = new UIConfiguration();
	menus = new UIWindowMenus();
	inspector = new UIInspector();
	gizmos = new UIGizmos();
	return ret;
}

bool M_UIManager::Start()
{
	return true;
}

update_status M_UIManager::PreUpdate(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->GetWindow());

	ImGuiIO& io = ImGui::GetIO();
	isUsingMouse = io.WantCaptureMouse;
	isUsingKeyboard = io.WantCaptureKeyboard;

	return UPDATE_CONTINUE;
}

update_status M_UIManager::Update(float dt)
{
	if (menus->imGuiDemo)
	{
		ImGui::ShowTestWindow();
	}
	
	return UPDATE_CONTINUE;
}

update_status M_UIManager::PostUpdate(float dt)
{
	console->Draw();
	frameCounter->DoUpdate(dt);
	frameCounter->Draw();
	inspector->Draw();
	if (outliner->active)
	{
		outliner->Draw();
	}
	if (tests->active)
	{
		tests->Draw();
	}
	configuration->DoUpdate(dt);
	if (configuration->active)
	{
		configuration->Draw();
	}
	menus->Draw();
	gizmos->Draw();
	
	return UPDATE_CONTINUE;
}

void M_UIManager::DrawEditor()
{
	ImGui::Render();
}

bool M_UIManager::CleanUp()
{
	ImGui_ImplSdlGL3_Shutdown();
	return true;
}

void M_UIManager::Serialize(Json::Value& root)
{
}

void M_UIManager::Deserialize(Json::Value& root)
{
}

void M_UIManager::LoadConfig()
{
}

void M_UIManager::SaveConfig()
{
}

void M_UIManager::setGreyStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.ChildWindowRounding = 3.f;
	style.GrabRounding = 0.f;
	style.WindowRounding = 6.f;
	style.ScrollbarRounding = 3.f;
	style.FrameRounding = 3.f;
	//style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

	style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.26f, 0.26f, 0.26f, 0.75f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.74f, 0.74f, 0.74f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.74f, 0.74f, 0.74f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.80f, 0.40f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.50f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.50f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.32f, 0.52f, 0.65f, 1.00f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);
}