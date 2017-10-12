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
			if (ImGui::MenuItem("Quit"))
			{
				App->quit = true;
			}
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
			ImGui::MenuItem("ImGui demo window", nullptr, &imGuiDemo, true);
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
	ImGuiWindowFlags creditsWindowFlags = 0;

	ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiSetCond_FirstUseEver);
	if (!ImGui::Begin("Credits", &creditsSelected, creditsWindowFlags))
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
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Separator();

	//#### LIBRARIES ####

	ImGui::Text("LIBRARIES");
	ImGui::Columns(3, "mixed");
	ImGui::Separator();

	ImColor color(84, 172, 255);
	ImGui::PushStyleColor(ImGuiCol_Button, color);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);

	//SDL
	ImGui::Text("SDL v2.0");
	//Image
	//Link
	if (ImGui::Button("Link##SDLb"))
	{
		App->OpenBrowser("https://www.libsdl.org/");
	}
	ImGui::NextColumn();

	//Jasoncpp
	ImGui::Text("JsonCpp v1.7.7");
	//Image
	//Link
	if (ImGui::Button("Link##JSONb"))
	{
		App->OpenBrowser("https://github.com/open-source-parsers/jsoncpp");
	}
	ImGui::NextColumn();

	//MathGeoLib
	ImGui::Text("MathGeoLib v1.5");
	//Image
	//Link
	if (ImGui::Button("Link##Mathb"))
	{
		App->OpenBrowser("https://github.com/juj/MathGeoLib");
	}
	ImGui::NextColumn();
	ImGui::Separator();

	//Assimp
	ImGui::Text("Assimp v3.3.1");
	//Image
	//Link
	if (ImGui::Button("Link##Assipb"))
	{
		App->OpenBrowser("http://assimp.sourceforge.net/");
	}
	ImGui::NextColumn();

	//Glew
	ImGui::Text("Glew v2.0.0");
	//Image
	//Link
	if (ImGui::Button("Link##Glewb"))
	{
		App->OpenBrowser("http://glew.sourceforge.net/");
	}
	ImGui::NextColumn();

	//PhysFS
	ImGui::Text("PhysFS v2.0.3");
	//Image
	//Link
	if (ImGui::Button("Link##PhysFSb"))
	{
		App->OpenBrowser("https://icculus.org/physfs/");
	}
	ImGui::NextColumn();
	ImGui::Separator();

	//ImGui
	ImGui::Text("ImGui v1.50");
	//Image
	//Link
	if (ImGui::Button("Link##ImGuib"))
	{
		App->OpenBrowser("https://github.com/ocornut/imgui");
	}
	ImGui::NextColumn();

	//OpenGL
	ImGui::Text("OpenGL v3.3");
	//Image
	//Link
	if (ImGui::Button("Link##OpenGLb"))
	{
		App->OpenBrowser("https://www.opengl.org/");
	}
	ImGui::NextColumn();

	//Devil
	ImGui::Text("Devil v1.7.8");
	//Image
	//Link
	if (ImGui::Button("Link##Devilb"))
	{
		App->OpenBrowser("http://openil.sourceforge.net/");
	}
	ImGui::NextColumn();

	ImGui::PopStyleColor(3);
	ImGui::Columns(1);
	ImGui::Separator();

	//Tanks!
	ImGui::Text("Thanks to all libraries shown above for making this project possible.");

	ImGui::Separator();
	ImGui::Separator();


	//#### LICENSE ####

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "License");
	ImGui::Text("This engine has been developed under the GNU General Public License."); ImGui::SameLine();
	if (ImGui::SmallButton("Link##Lice"))
	{
		App->OpenBrowser("https://www.gnu.org/licenses/gpl-3.0.en.html");
	}
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	static char text[1024 * 3] =
		" _   _ ____ ____    ____ ____ ___ _  _ ____ ___ _  _\n"
		"  \\_/  |  | | __ __ [__  |  |  |  |__| |  |  |  |__|\n"
		"   |   |__| |__]    ___] |__|  |  |  | |__|  |  |  |\n"
		"                                                 Engine\n";

	//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
	//ImGui::PopStyleVar();
	ImGui::InputTextMultiline("##YogLogo", text, IM_ARRAYSIZE(text), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 7), ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_ReadOnly);
	
	ImGui::End();
}
