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

	console = new UIConsole();
	console->active = true;
	frameCounter = new UIFrameCounter();
	frameCounter->active = true;
	outliner = new UIOutliner();
	tests = new UITests();
	menus = new UIWindowMenus();
	return ret;
}

bool M_UIManager::Start()
{
	return true;
}

update_status M_UIManager::PreUpdate(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->GetWindow());
	return UPDATE_CONTINUE;
}

update_status M_UIManager::Update(float dt)
{
	ImGui::ShowTestWindow();
	
	return UPDATE_CONTINUE;
}

update_status M_UIManager::PostUpdate(float dt)
{
	console->Draw();
	frameCounter->DoUpdate(dt);
	frameCounter->Draw();
	if (outliner->active)
	{
		outliner->Draw();
	}
	if (tests->active)
	{
		tests->Draw();
	}
	menus->Draw();

	
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
