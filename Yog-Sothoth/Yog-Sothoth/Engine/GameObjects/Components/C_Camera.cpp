#include "C_Camera.h"
#include "../../Application.h"
#include "../../Engine/CoreModules/M_Input.h"

C_Camera::C_Camera(GameObject* parent) : Component(parent)
{
	camera.type = PerspectiveFrustum;
	camera.pos = { 0.f, 0.f, 20.f };
	camera.up = { 0.f, 1.f, 0.f };
	camera.front = { 0.f, 0.f, 1.f };
	camera.horizontalFov = 1.309f;
	camera.verticalFov = 0.82f;
	camera.nearPlaneDistance = 0.001f;
	camera.farPlaneDistance = 1000.f;
}

C_Camera::~C_Camera()
{
}

void C_Camera::Move(float dt)
{
	Frustum* frust = &camera;

	float speed;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = movSpeed / 2;
	else
		speed = movSpeed;

	float3 movement(float3::zero);
	float3 forw(frust->front);
	float3 right(frust->WorldRight());

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) movement += forw;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) movement -= forw;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) movement += right;
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) movement -= right;
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) movement += float3::unitY;
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) movement -= float3::unitY;

	if (!movement.Equals(float3::zero))
	{
		movement *= (speed * dt);
		frust->pos = frust->pos + movement;
	}
}

void C_Camera::Rotate(float dt)
{
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int motX = App->input->GetMouseXMotion();
		int motY = App->input->GetMouseYMotion();

		if (motX == 0 && motY == 0)
			return;

		float speed;
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed = rotSpeed / 2;
		else
			speed = rotSpeed;

		float x = (float)-motX * speed * dt;
		float y = (float)-motY * speed * dt;

//		if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
//			Orbit(x, y);
//		else
		LookAt(x, y);
	}
}

void C_Camera::LookAt(float dx, float dy)
{
	//dx will be rotation along x axis
	if (dx != 0.f)
	{
		Quat rot = Quat::RotateY(dx);
		camera.front = rot.Mul(camera.front).Normalized();
		camera.up = rot.Mul(camera.up).Normalized();
	}

	//dy will be rotation along y axis
	//more complex as the frustum up changes
	if (dy != 0.f)
	{
		Quat rot = Quat::RotateAxisAngle(camera.WorldRight(), dy);

		camera.front = rot.Mul(camera.front).Normalized();
		camera.up = rot.Mul(camera.up).Normalized();
	}

}
