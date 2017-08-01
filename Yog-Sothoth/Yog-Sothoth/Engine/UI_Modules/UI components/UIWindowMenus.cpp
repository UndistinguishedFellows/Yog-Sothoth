#include "UIWindowMenus.h"
#include "../../../imGUI/imgui.h"
#include "../../../Application.h"

UIWindowMenus::UIWindowMenus()
{
}


UIWindowMenus::~UIWindowMenus()
{
}

void UIWindowMenus::Draw()
{
	ImGui::ShowTestWindow();


	if (aboutSelected)
	{
		ImGui::Begin("About", &aboutSelected);
		ImGui::Text("A game engine made for educational purposes by CapitanLiteral");
		ImGui::End();
	}
	if (openReleaseDirectory)
	{
		App->OpenBrowser("https://github.com/CapitanLiteral/Wingman/releases");
		openReleaseDirectory = false;
	}
	if (openRepoDirectory)
	{
		App->OpenBrowser("https://github.com/CapitanLiteral/Wingman");
		openRepoDirectory = false;
	}
	if (creditsSelected)
	{
		ShowCredits();
		//App->jsonParser->print_commits_info("CapitanLiteral", "Wingman");
	}

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
//			ImGui::MenuItem("Save", NULL, &App->goManager->haveToSaveScene, true);
//			ImGui::MenuItem("Load", NULL, &App->goManager->haveToLoadScene, true);
//			ImGui::MenuItem("Quit", NULL, &quitSelected, true);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Windows"))
		{
//			ImGui::MenuItem("FPS info", NULL, &showfpsInfo, true);
			ImGui::MenuItem("Outliner", nullptr, &App->uiManager->outliner->active, true);
//			ImGui::MenuItem("Inspector", NULL, &App->editor->show_inspector, true);
//			ImGui::MenuItem("Settings", NULL, &App->editor->show_settings, true);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			ImGui::MenuItem("About", nullptr, &aboutSelected, true);
			ImGui::MenuItem("Releases", nullptr, &openReleaseDirectory, true);
			ImGui::MenuItem("Repository", nullptr, &openRepoDirectory, true);
			ImGui::MenuItem("Credits", nullptr, &creditsSelected, true);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}


	if (quitSelected) //Aplication closing
	{
		SDL_Log("Closing aplication from editor");
		QUIT = true;
	}
}

void UIWindowMenus::ShowCredits()
{
	ImGuiWindowFlags outilnerWindowFlags = 0;
	outilnerWindowFlags |= ImGuiWindowFlags_AlwaysHorizontalScrollbar;

	ImGui::SetNextWindowSize(ImVec2(300, 600), ImGuiSetCond_FirstUseEver);
	if (!ImGui::Begin("Credits", &credits, outilnerWindowFlags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}
	ImVec4 acolor(128, 137, 237, 0);
	ImColor color(84, 172, 255);
	//SDL
	ImGui::Text("SDL");
	//Image
	//Link
	ImGui::PushStyleColor(ImGuiCol_Button, color);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
	if (ImGui::Button("Link"))
	{
		App->OpenBrowser("https://www.libsdl.org/");
	}
	ImGui::PopStyleColor(3);
	//Tanks!
	ImGui::Text("Thanks to SDL library to make this project possible");

	ImGui::Separator();
	//Jasoncpp
	//Image
	//Link
	//Tanks!

	ImGui::Separator();
	//Bullet
	//Image
	//Link
	//Tanks!

	ImGui::Separator();
	//MathGeoLib
	//Image
	//Link
	//Tanks!

	ImGui::Separator();
	//Assimp
	//Image
	//Link
	//Tanks!

	ImGui::Separator();
	//Devil
	//Image
	//Link
	//Tanks!

	ImGui::Separator();
	//Glew
	//Image
	//Link
	//Tanks!

	ImGui::Separator();
	//PhysFS
	//Image
	//Link
	//Tanks!



	ImGui::End();
}
