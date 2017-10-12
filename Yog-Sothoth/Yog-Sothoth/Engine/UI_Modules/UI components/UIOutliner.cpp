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

	if (ImGui::TreeNodeEx(App->objManager->root->name.c_str(), flags))
	{
		if (root)
		{
			std::vector<GameObject*> children = root->children;
			for (std::vector<GameObject*>::iterator it = children.begin();
				 it != children.end(); ++it)
			{
				TreeNodes((*it));
			}
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
		}
		else
			nodeFlags |= ImGuiTreeNodeFlags_Leaf;

		if (ImGui::TreeNodeEx(node->name.c_str(), nodeFlags))
		{
			if (ImGui::IsItemClicked())
			{
				if (App->objManager->GetFocusGO() != nullptr)
				{
					App->objManager->GetFocusGO()->selected = false;
				}
				App->objManager->SetFocusGO(node);
				node->selected = true;
			}
			for (uint i = 0; i < node->children.size(); ++i)
			{
				TreeNodes(node->children[i]);
			}
			ImGui::TreePop();
		}
	}

}
