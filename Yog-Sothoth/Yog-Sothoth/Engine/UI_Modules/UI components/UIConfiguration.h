#pragma once
#include "../../../imGUI/imgui.h"
#include "BaseClass/UIComponent.h"
#include <vector>

class UIConfiguration : public UIComponent
{
public:
	UIConfiguration();
	~UIConfiguration();

	void DoUpdate(float dt) override;
	void Draw() override;

	ImGuiWindowFlags testsWindowFlags = 0;

	std::vector<float> fpsLog;
	std::vector<float> msLog;
	std::vector<float> memLog;
};