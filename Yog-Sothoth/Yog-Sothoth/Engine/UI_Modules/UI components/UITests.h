#pragma once
#include "../../../imGUI/imgui.h"
#include "BaseClass/UIComponent.h"
#include "../../Test/YogTest.h"

class GameObject;

class UITests : public UIComponent
{
public:
	UITests();
	~UITests();

	void Draw() override;

	ImGuiWindowFlags testsWindowFlags = 0;
	YogTest* yogTest = nullptr;

};