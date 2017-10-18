#pragma once
#include "../../../im3d/im3d.h"
#include "BaseClass/UIComponent.h"

class UIGizmos : public UIComponent
{
public:

	void DoUpdate(float dt) override;
	void Draw() override;
};
