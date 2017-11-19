#include "UITimeControllers.h"
#include "../../../Application.h"

UITimeControllers::UITimeControllers()
{
	active = true;
}

UITimeControllers::~UITimeControllers()
{
}

void UITimeControllers::Draw()
{
	ImGui::SetNextWindowPos(ImVec2(650, 30));
	ImGuiWindowFlags testsWindowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

	ImGui::SetNextWindowSize(ImVec2(250, 20), ImGuiSetCond_FirstUseEver);

	if (!ImGui::Begin("##TimeControllers", &active, testsWindowFlags))
	{
		ImGui::End();
		return;
	}

	if (App->gameStatus == game_status::STOP)
	{
		if (ImGui::Button("Play"))
		{
			App->gameStatus = game_status::TO_PLAY;
		}
	}

	if (App->gameStatus == game_status::PLAY || App->gameStatus == game_status::PAUSE)
	{
		if (App->gameStatus == game_status::PAUSE)
		{
			if (ImGui::Button("Continue"))
			{
				App->gameStatus = game_status::PLAY;
			}
			ImGui::SameLine();
		}
		else
		{
			if (ImGui::Button("Pause"))
			{
				App->gameStatus = game_status::PAUSE;
			}
			ImGui::SameLine();
		}

		if (ImGui::Button("Stop"))
		{
			App->gameStatus = game_status::TO_STOP;
			App->appTimer.Stop();
		}

		ImGui::SameLine();
		ImGui::Text("Game DT: %.3f", App->appTimer.GetGameDT());
	}

	ImGui::End();
}