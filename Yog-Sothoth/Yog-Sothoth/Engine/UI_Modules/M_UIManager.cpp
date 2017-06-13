﻿#include "M_UIManager.h"
#include "../../Globals.h"
#include "../../Application.h"
#include <SDL.h>
#include "../../imGUI/imgui.h"
#include "../../imGUI/imgui_impl_sdl_gl3.h"
#include "../../OpenGL.h"


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
	ImGui::Render();
	return UPDATE_CONTINUE;
}

bool M_UIManager::CleanUp()
{
	ImGui_ImplSdlGL3_Shutdown();
	return true;
}