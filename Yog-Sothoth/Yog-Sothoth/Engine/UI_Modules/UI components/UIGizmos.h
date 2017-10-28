#pragma once
#include "BaseClass/UIComponent.h"

class UIGizmos : public UIComponent
{
public:

	void DoUpdate(float dt) override;
	void Draw() override;
};
