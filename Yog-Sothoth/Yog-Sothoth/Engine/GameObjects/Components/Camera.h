#pragma once

#include "BaseClass/Component.h"
#include "../../MathGeoLib/MathGeoLib.h"

class Camera : public Component
{
public:
	Camera();
	~Camera();

	Frustum camera;
};
