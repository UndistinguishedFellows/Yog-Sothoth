#include "C_Camera.h"
#include "../../Application.h"
#include "../../Engine/CoreModules/M_Input.h"

C_Camera::C_Camera(GameObject* parent) : Component(parent)
{
	camera.type = PerspectiveFrustum;
	//camera.pos = { -10.f, 10.f, 20.f };
	camera.up = { 0.f, 1.f, 0.f };
	camera.front = { 0.f, 0.f, -1.f };
	camera.horizontalFov = DegToRad(45);
	camera.verticalFov = DegToRad(45/ aspectRatio);
//	camera.horizontalFov = 1.309f;
//	camera.verticalFov = 0.82f;
	camera.nearPlaneDistance = 0.001f;
	camera.farPlaneDistance = 1000.f;
	//LookAt(float3(0, 0, 0));
}

C_Camera::~C_Camera()
{
}

void C_Camera::Move(float dt)
{
	C_Transform* transform = (C_Transform*)parent->FindComponent(C_TRANSFORM);
	if (transform != nullptr)
	{
		float3 pos;
		float3 sca;
		Quat rot;
		transform->localTransform.Decompose(pos, rot, sca);

		Frustum* frust = &camera;

		float speed;
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed = movSpeed / 2;
		else
			speed = movSpeed;

		float3 movement(float3::zero);
		float3 forw(frust->front);
		float3 right(frust->WorldRight());
		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) movement += forw;
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) movement -= forw;
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) movement += right;
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) movement -= right;
			if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) movement += float3::unitY;
			if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) movement -= float3::unitY;
		}

		if (!movement.Equals(float3::zero))
		{
			movement *= (speed * dt);
			frust->pos = pos + movement;
			transform->localTransform = float4x4::FromTRS(pos + movement, rot, sca);
		}
		transform->RefreshTransform();
	}
}

void C_Camera::Rotate(float dt)
{
	C_Transform* transform = (C_Transform*)parent->FindComponent(C_TRANSFORM);
	if (transform != nullptr)
	{
		camera.front = transform->localTransform.WorldZ();
		camera.up = transform->localTransform.WorldY();

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
}

void C_Camera::LookAt(float dx, float dy)
{
	//dx will be rotation along x axis
	C_Transform* transform = (C_Transform*)parent->FindComponent(C_TRANSFORM);
	if (transform != nullptr)
	{
		
		if (dx != 0.f)
		{
			Quat roty = Quat::RotateY(dx);
			float3x3 matrot = float3x3::identity;
			float3 pos;
			float3 sca;
			Quat rot;
			transform->localTransform.Decompose(pos, rot, sca);
			matrot = matrot.Mul(matrot.FromQuat(roty));
			matrot = matrot.Mul(transform->localTransform.RotatePart());
			transform->localTransform.SetRotatePart(matrot);

			camera.front = transform->localTransform.WorldZ().Normalized();
			camera.up = transform->localTransform.WorldY().Normalized();
			transform->RefreshTransform();
		}

		//dy will be rotation along y axis
		//more complex as the frustum up changes
		if (dy != 0.f)
		{
			Quat rot = Quat::RotateAxisAngle(camera.WorldRight(), dy);
			transform->localTransform = transform->localTransform.Mul(transform->localTransform.RotateX(-dy));
			camera.front = transform->localTransform.WorldZ();
			camera.up = transform->localTransform.WorldY();
			transform->RefreshTransform();
		}
		
	}

}
void C_Camera::LookAt(const float3 spot)
{
	C_Transform* transform = (C_Transform*)parent->FindComponent(C_TRANSFORM);
	if (transform != nullptr)
	{
		float3 dir = spot - camera.pos;
		float3x3 mat = float3x3::LookAt(camera.front, dir.Normalized(), camera.up, float3::unitY);

		mat = mat.Mul(transform->localTransform.RotatePart());
		transform->localTransform.SetRotatePart(mat);

		camera.front = transform->localTransform.WorldZ().Normalized();
		camera.up = transform->localTransform.WorldY().Normalized();
		transform->RefreshTransform();

	}
}

void C_Camera::FocusCamera(GameObject* focus)
{
	if (focus == nullptr)
	{
		focus = App->objManager->GetFocusGO();
	}
	if (focus != nullptr)
	{
		C_Mesh* mesh = (C_Mesh*)focus->FindComponent(C_MESH);
		C_Transform* transform = (C_Transform*)parent->FindComponent(C_TRANSFORM);

		if (mesh != nullptr && transform != nullptr)
		{
			AABB focusAABB;
			focusAABB.SetNegativeInfinity();
			focusAABB.Enclose((float3*)mesh->vertices.vertices, mesh->vertices.numVertices);

			float3 centerPoint = focusAABB.CenterPoint();
			float3 size = focusAABB.Size();

			float cameraDist_y = (size.y/2) / math::Tan(camera.verticalFov/2);
			float cameraDist_x = (size.x/2) / math::Tan(camera.horizontalFov/2);

			if (cameraDist_y > cameraDist_x)
			{
				transform->SetPosition(float3(0, centerPoint.y, cameraDist_y));
			}
			else
			{
				transform->SetPosition(float3(0, centerPoint.y, cameraDist_x));
			}
			//LookAt(centerPoint);
			transform->RefreshTransform();
		}
	}

}

void C_Camera::Zoom(float dt)
{
	Frustum* frust = &camera;
	float3 movement(float3::zero);
	float3 forw(frust->front);

	if (App->input->GetMouseZ() > 0)
	{
		movement += forw;
	}
	else if (App->input->GetMouseZ() < 0)
	{
		movement -= forw;
	}
	if (!movement.Equals(float3::zero))
	{
		movement *= (movSpeed * 20 * dt);
		frust->pos = frust->pos + movement;
	}

}
