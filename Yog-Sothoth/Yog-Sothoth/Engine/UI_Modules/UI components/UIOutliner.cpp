#include "UIOutliner.h"
#include "../../../Application.h"

UIOutliner::UIOutliner()
{
	active = true;
}

UIOutliner::~UIOutliner()
{
}

void UIOutliner::Draw()
{
	////////////////////////////////////////////////////////////////////////////////////////////////

	ImGuiWindowFlags outilnerWindowFlags = 0;
	outilnerWindowFlags |= ImGuiWindowFlags_AlwaysHorizontalScrollbar;
	outilnerWindowFlags |= ImGuiWindowFlags_NoMove;
	//outilnerWindowFlags |= ImGuiWindowFlags_NoResize;

	ImGui::SetNextWindowSize(ImVec2(300, 600), ImGuiSetCond_FirstUseEver);
	if (!ImGui::Begin("New Outliner", &active, outilnerWindowFlags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}
	ImGui::Spacing();

	//const GameObject* root = App->objManager->root;
	const GameObject* root;
	if (App->renderer->fbxViewer)
		root = App->objManager->dragAndDropVisualizer;
	else
		root = App->objManager->root;


	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnDoubleClick;
	flags |= ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= ImGuiTreeNodeFlags_DefaultOpen;


	if (ImGui::TreeNodeEx("Scene", flags))
	{
		if (root)
		{
			TreeNodes(App->objManager->root);
			/*std::vector<GameObject*> children = root->children;
			for (std::vector<GameObject*>::iterator it = children.begin();
				 it != children.end(); ++it)
			{
				TreeNodes((*it));
			}*/
		}

		ImGui::TreePop();
	}


	ImGui::End();

	/////////////////////////////////////////////////////////////////////////////////////////////////

}

void UIOutliner::TreeNodes(GameObject* node)
{
	if (node != nullptr)
	{
		ImGuiTreeNodeFlags nodeFlags = 0;
		if (node->children.size() > 0)
		{
			nodeFlags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
			nodeFlags |= ImGuiTreeNodeFlags_OpenOnArrow;
			nodeFlags |= ImGuiTreeNodeFlags_DefaultOpen;
		}
		else
			nodeFlags |= ImGuiTreeNodeFlags_Leaf;

		if (node->selected)
		{
			nodeFlags |= ImGuiTreeNodeFlags_Selected;
		}
		if (ImGui::TreeNodeEx(node->name.c_str(), nodeFlags))
		{
			if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1))
			{
				if (App->objManager->GetFocusGO() != nullptr)
				{
					App->objManager->GetFocusGO()->selected = false;
				}
				App->objManager->SetFocusGO(node);
				node->selected = true;

				if (ImGui::IsItemClicked(1))
				{
					ImGui::OpenPopup("secondaryMenu");
				}

			}
			if (ImGui::BeginPopup("secondaryMenu"))
			{
				bool enabler;
				enabler = true;

				if (ImGui::BeginMenu("Add GameObject"))
				{
					//Prefab List
					if(ImGui::MenuItem("Empty"))
					{
						App->objManager->CreateGameObject(App->objManager->GetFocusGO());
					}
					if (ImGui::BeginMenu("With Prefab"))
					{
						//Prefab List
						ImGui::MenuItem("Prefab 1");
						ImGui::MenuItem("Prefab 2");
						ImGui::MenuItem("Prefab 3");
						ImGui::MenuItem("Prefab 4");
						ImGui::EndMenu();
					}
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Assign Prefab"))
				{
					//Prefab List
					ImGui::MenuItem("Prefab 1");
					ImGui::MenuItem("Prefab 2");
					ImGui::MenuItem("Prefab 3");
					ImGui::MenuItem("Prefab 4");
					ImGui::EndMenu();
				}

				if (App->objManager->GetFocusGO() == App->objManager->root
					|| App->objManager->GetFocusGO() == App->objManager->activeCamera
					|| App->objManager->GetFocusGO() == App->objManager->cullingCamera)
				{
					enabler = false;
				}

				if (ImGui::BeginMenu("Rename"))
				{
					static char goName[128];
					static char newGoName[128];
					static bool edited = false;

					strcpy_s(goName, 128, App->objManager->GetFocusGO()->name.c_str());

					if (!edited)
					{
						strcpy_s(newGoName, 128, goName);
					}

					if (ImGui::InputText("##goName", goName, 128))
					{
						edited = true;
						strcpy_s(newGoName, 128, goName);
					}

					ImGui::SameLine();

					if (ImGui::Button("Apply")){
						edited = false;
						App->objManager->GetFocusGO()->name = newGoName;
					}
					ImGui::EndMenu();
				}

				if (ImGui::MenuItem("Delete", "", false, enabler))
				{
					GameObject* tmp = App->objManager->GetFocusGO();
					App->objManager->SetFocusGO(nullptr);
					App->objManager->DeleteGameObject(tmp);
				}
				enabler = true;

				ImGui::EndPopup();
			}
			for (uint i = 0; i < node->children.size(); ++i)
			{
				TreeNodes(node->children[i]);
			}
			ImGui::TreePop();
		}
	}

}
