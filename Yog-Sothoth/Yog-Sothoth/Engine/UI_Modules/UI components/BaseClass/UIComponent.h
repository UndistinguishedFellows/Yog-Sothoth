#pragma once

class UIComponent
{
public:
	UIComponent(){}
	virtual ~UIComponent(){}

	virtual void DoUpdate(float dt) {};

	virtual void Draw() = 0;
};