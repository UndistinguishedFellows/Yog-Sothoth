#include "Camera.h"

Camera::Camera()
{
	camera.type = PerspectiveFrustum;
	camera.pos = { 0.f, 0.f, 20.f };
	camera.up = { 0.f, 1.f, 0.f };
	camera.front = { 0.f, 0.f, 1.f };
	camera.horizontalFov = 1.309;
	camera.verticalFov = 0.82f;
	camera.nearPlaneDistance = 0.001f;
	camera.farPlaneDistance = 1000.f;


}

Camera::~Camera()
{
}
