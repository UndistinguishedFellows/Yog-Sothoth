#pragma once
#include "BaseClass/UIComponent.h"

class UIWaterShader : public UIComponent
{
public:

	void DoUpdate(float dt) override;
	void Draw() override;
	
};
