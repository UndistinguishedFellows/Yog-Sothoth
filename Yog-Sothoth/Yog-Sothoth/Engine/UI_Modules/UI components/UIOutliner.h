#pragma once
#include "../../../imGUI/imgui.h"
#include "BaseClass/UIComponent.h"

class GameObject;

class UIOutliner : public UIComponent
{
public:
	UIOutliner();
	~UIOutliner();

	void Draw() override;
	void TreeNodes(GameObject* node);
	void ListPrefabs();

	ImGuiWindowFlags outilnerWindowFlags = 0;

};
