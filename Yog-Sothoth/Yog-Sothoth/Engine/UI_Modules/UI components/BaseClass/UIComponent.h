#pragma once

class UIComponent
{
public:
	bool active = false;

	UIComponent(){}
	virtual ~UIComponent(){}

	virtual void DoUpdate(float dt) {};

	virtual void Draw() = 0;
};