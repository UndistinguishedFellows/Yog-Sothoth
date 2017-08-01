#pragma once
#include "BaseClass/UIComponent.h"
#include "../../imGUI/imgui.h"
class UIFrameCounter :
	public UIComponent
{
public:
	UIFrameCounter();
	virtual ~UIFrameCounter();

	void DoUpdate(float dt) override;

	void Draw() override;

	bool pOpen = false;
	float dt = 0;
	float fps = 0;

};

