#pragma once

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
	void Orbit(float dt);
	void LookAt(const float3 spot);
	void FocusCamera(GameObject* focus = nullptr);
	void Zoom(float dt);
	std::vector<GameObject*> GetElementsToDraw();
	void DrawDebug();

	Frustum frustum;

	float movSpeed = 5.f;
	float rotSpeed = 1.2f;
	float zoomSpeed = 500.f;
	float aspectRatio = 1.77f;

	bool editorCamera = false;
	bool cameraCulling = false;

	float3 customOrbitPoint = float3(0,0,0);
};
