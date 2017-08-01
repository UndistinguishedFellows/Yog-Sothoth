#pragma once
#include "../../../imGUI/imgui.h"

class UIOutliner
{
public:
	UIOutliner();
	virtual ~UIOutliner();

	void draw();
	void treeNodes(GameObject* node);

	ImGuiWindowFlags outilnerWindowFlags = 0;

};
