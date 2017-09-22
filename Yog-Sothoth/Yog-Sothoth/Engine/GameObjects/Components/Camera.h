#pragma once

#include "BaseClass/Component.h"
#include "../../MathGeoLib/MathGeoLib.h"

class Camera : public Component
{
public:
	Camera();
	~Camera();

	void Move(float dt);
	void Rotate(float dt);
	void LookAt(float dx, float dy);

	Frustum camera;

	float movSpeed = 5.f;
	float rotSpeed = 2.f;
	float zoomSpeed = 500.f;
};
