#include "C_Camera.h"
#include "../../Application.h"
#include "../../Engine/CoreModules/M_Input.h"
#include "C_Transform.h"
#include "C_Mesh.h"

C_Camera::C_Camera(GameObject* parent) : Component(parent)
{
	frustum.type = PerspectiveFrustum;
	//camera.pos = { -10.f, 10.f, 20.f };
	frustum.up = { 0.f, 1.f, 0.f };
	frustum.front = { 0.f, 0.f, -1.f };
	frustum.horizontalFov = DegToRad(45);
	frustum.verticalFov = DegToRad(45/ aspectRatio);
//	camera.horizontalFov = 1.309f;
//	camera.verticalFov = 0.82f;
	frustum.nearPlaneDistance = 0.001f;
	frustum.farPlaneDistance = 1000.f;
	//LookAt(float3(0, 0, 0));
}

C_Camera::~C_Camera()
{
}

void C_Camera::Move(float dt)
{
	if (ownerParent->Transform != nullptr)
	{
		float3 pos;
		float3 sca;
		Quat rot;
		ownerParent->Transform->localTransform.Decompose(pos, rot, sca);

		Frustum* frust = &frustum;

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
			ownerParent->Transform->localTransform = float4x4::FromTRS(pos + movement, rot, sca);
		}
		ownerParent->Transform->RefreshTransform();
	}
}

void C_Camera::Rotate(float dt)
{
	if (ownerParent->Transform != nullptr)
	{
		frustum.front = ownerParent->Transform->localTransform.WorldZ();
		frustum.up = ownerParent->Transform->localTransform.WorldY();

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
	if (ownerParent->Transform != nullptr)
	{
		
		if (dx != 0.f)
		{
			Quat roty = Quat::RotateY(dx);
			float3x3 matrot = float3x3::identity;
			float3 pos;
			float3 sca;
			Quat rot;
			ownerParent->Transform->localTransform.Decompose(pos, rot, sca);
			matrot = matrot.Mul(matrot.FromQuat(roty));
			matrot = matrot.Mul(ownerParent->Transform->localTransform.RotatePart());
			ownerParent->Transform->localTransform.SetRotatePart(matrot);

			frustum.front = ownerParent->Transform->localTransform.WorldZ().Normalized();
			frustum.up = ownerParent->Transform->localTransform.WorldY().Normalized();
			ownerParent->Transform->RefreshTransform();
		}

		//dy will be rotation along y axis
		//more complex as the frustum up changes
		if (dy != 0.f)
		{
			Quat rot = Quat::RotateAxisAngle(frustum.WorldRight(), dy);
			ownerParent->Transform->localTransform = ownerParent->Transform->localTransform.Mul(ownerParent->Transform->localTransform.RotateX(-dy));
			frustum.front = ownerParent->Transform->localTransform.WorldZ();
			frustum.up = ownerParent->Transform->localTransform.WorldY();
			ownerParent->Transform->RefreshTransform();
		}
		
	}

}
void C_Camera::LookAt(const float3 spot)
{
	if (ownerParent->Transform != nullptr)
	{
		float3 dir = spot - frustum.pos;
		float3x3 mat = float3x3::LookAt(frustum.front, dir.Normalized(), frustum.up, float3::unitY);

		mat = mat.Mul(ownerParent->Transform->localTransform.RotatePart());
		ownerParent->Transform->localTransform.SetRotatePart(mat);

		frustum.front = ownerParent->Transform->localTransform.WorldZ().Normalized();
		frustum.up = ownerParent->Transform->localTransform.WorldY().Normalized();
		ownerParent->Transform->RefreshTransform();

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
		if (ownerParent->Mesh != nullptr && ownerParent->Transform != nullptr)
		{
			AABB focusAABB;
			focusAABB.SetNegativeInfinity();
			focusAABB.Enclose((float3*)ownerParent->Mesh->vertices.vertices, ownerParent->Mesh->vertices.numVertices);

			float3 centerPoint = focusAABB.CenterPoint();
			float3 size = focusAABB.Size();

			float cameraDist_y = (size.y/2) / math::Tan(frustum.verticalFov/2);
			float cameraDist_x = (size.x/2) / math::Tan(frustum.horizontalFov/2);

			if (cameraDist_y > cameraDist_x)
			{
				ownerParent->Transform->SetPosition(float3(0, centerPoint.y, cameraDist_y));
			}
			else
			{
				ownerParent->Transform->SetPosition(float3(0, centerPoint.y, cameraDist_x));
			}
			//LookAt(centerPoint);
			ownerParent->Transform->RefreshTransform();
		}
	}

}

void C_Camera::Zoom(float dt)
{
	Frustum* frust = &frustum;
	float3 movement(float3::zero);
	float3 forw(frust->front);
	float3 pos;
	float3 sca;
	Quat rot;
	ownerParent->Transform->localTransform.Decompose(pos, rot, sca);

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
		ownerParent->Transform->localTransform = float4x4::FromTRS(frust->pos, rot,sca );
	}
}
