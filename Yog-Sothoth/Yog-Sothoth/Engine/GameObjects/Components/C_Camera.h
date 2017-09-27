﻿#pragma once

#include "BaseClass/Component.h"
#include "../../MathGeoLib/MathGeoLib.h"

class C_Camera : public Component
{
public:
	C_Camera(GameObject* parent);
	~C_Camera();

	void Move(float dt);
	void Rotate(float dt);
	void LookAt(float dx, float dy);

	Frustum camera;

	float movSpeed = 5.f;
	float rotSpeed = 1.2f;
	float zoomSpeed = 500.f;
};