#include "C_Camera.h"
#include "../../Application.h"
#include "../../Engine/CoreModules/M_Input.h"
#include "C_Transform.h"
#include "C_Mesh.h"

C_Camera::C_Camera(GameObject* parent) : Component(parent)
{
	frustum.type = PerspectiveFrustum;
	frustum.up = { 0.f, 1.f, 0.f };
	frustum.front = { 0.f, 0.f, -1.f };
	frustum.horizontalFov = DegToRad(45);
	frustum.verticalFov = DegToRad(45/ aspectRatio);
	frustum.nearPlaneDistance = 1.f;
	frustum.farPlaneDistance = 1000.f;
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
			if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) movement += float3::unitY;
			if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) movement -= float3::unitY;
		}
		else
		{
			if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) movement += forw;
			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) movement -= forw;
			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) movement += right;
			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) movement -= right;
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
		frustum.front = ownerParent->Transform->localTransform.WorldZ().Normalized();
		frustum.up = ownerParent->Transform->localTransform.WorldY().Normalized();

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
			frustum.front = ownerParent->Transform->localTransform.WorldZ().Normalized();
			frustum.up = ownerParent->Transform->localTransform.WorldY().Normalized();
			ownerParent->Transform->RefreshTransform();
		}
		
	}

}

void C_Camera::Orbit(float dt)
{
	int motX = App->input->GetMouseXMotion();
	int motY = App->input->GetMouseYMotion();
	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		float3 orbitPoint;
		if (App->objManager->GetFocusGO() == nullptr)
		{
			orbitPoint = customOrbitPoint;
		}
		else
		{
			orbitPoint = App->objManager->GetFocusGO()->aabb.CenterPoint();
		}

		LookAt(orbitPoint);
		float dist = Distance(frustum.pos, orbitPoint);
		float3 distf3 = frustum.front.Normalized() * dist;

		float3 pos = ownerParent->Transform->localTransform.TranslatePart();
		ownerParent->Transform->localTransform.SetTranslatePart(pos + distf3);
		frustum.pos = pos + distf3;

		LookAt(motX * dt, motY * dt);

		distf3 = frustum.front.Normalized() * dist;
		pos = ownerParent->Transform->localTransform.TranslatePart();
		ownerParent->Transform->localTransform.SetTranslatePart(pos - distf3);
		frustum.pos = pos - distf3;
		
		frustum.front = ownerParent->Transform->localTransform.WorldZ().Normalized();
		frustum.up = ownerParent->Transform->localTransform.WorldY().Normalized();

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
		if (ownerParent->Transform != nullptr)
		{
			float3 centerPoint = focus->aabb.CenterPoint();
			float3 size = focus->aabb.Size();

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
			LookAt(centerPoint);
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


