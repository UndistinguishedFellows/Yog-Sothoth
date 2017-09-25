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
	if (openReleaseDirectory)
	{
		App->OpenBrowser("https://github.com/UndistinguishedFellows/Yog-Sothoth/releases");
		openReleaseDirectory = false;
	}
	if (openRepoDirectory)
	{
		App->OpenBrowser("https://github.com/UndistinguishedFellows/Yog-Sothoth");
		openRepoDirectory = false;
	}
	if (openIssuesDirectory)
	{
		App->OpenBrowser("https://github.com/UndistinguishedFellows/Yog-Sothoth/issues");
		openIssuesDirectory = false;
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
			ImGui::MenuItem("Configuration", nullptr, &App->uiManager->configuration->active, true);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			ImGui::MenuItem("Report an Issue", nullptr, &openIssuesDirectory, true);
			ImGui::MenuItem("Tests", nullptr, &App->uiManager->tests->active, true);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("About"))
		{
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

	//#### DESCRIPTION & AUTHORS ####

	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Yog-Sothoth Engine");
	ImGui::Text("A game engine made for educational purposes by:");
	ImGui::BulletText("CapitanLiteral"); ImGui::SameLine();
	if (ImGui::SmallButton("Link##Capi"))
	{
		App->OpenBrowser("https://github.com/CapitanLiteral");
	}
	ImGui::BulletText("dibu13"); ImGui::SameLine();
	if (ImGui::SmallButton("Link##Dibu"))
	{
		App->OpenBrowser("https://github.com/dibu13");
	}

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();


	//#### LIBRARIES ####

	ImColor color(84, 172, 255);
	//SDL
	ImGui::Text("SDL");
	//Image
	//Link
	ImGui::PushStyleColor(ImGuiCol_Button, color);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
	if (ImGui::Button("Link##SDLb"))
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

	//#### LICENSE ####

	ImGui::Separator();
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "License");
	ImGui::Text("This engine has been developed under the GNU General Public License."); ImGui::SameLine();
	if (ImGui::SmallButton("Link##Lice"))
	{
		App->OpenBrowser("https://www.gnu.org/licenses/gpl-3.0.en.html");
	}

	ImGui::End();
}
