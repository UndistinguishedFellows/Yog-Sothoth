#pragma once
#include "../../../imGUI/imgui.h"
#include "BaseClass/UIComponent.h"

class UITimeControllers : public UIComponent
{
public:
	UITimeControllers();
	~UITimeControllers();

	void Draw() override;
};