#pragma once
#include "BaseClass/Component.h"
#include "../../../MathGeoLib/MathGeoLib.h"
#include "../../../Tools/Containers/Color.h"

class C_Transform :
	public Component
{
public:
	// Father dependant transform
	float4x4 localTransform = float4x4::identity;
	// C_Transform dependant from the origin
	float4x4 globalTransform = float4x4::identity;
	float3 scale;
	float3 position;
	Quat rotation;

	AABB aabb;
	Color aabb_color;
	
	C_Transform(GameObject* parent);
	~C_Transform();

	const float4x4 GetLocalTransform();
	const float4x4 GetGlobalTransform();

	void RefreshTransform();

	void SetPosition(float3 position);
	void SetRotation(Quat rotation);
	void SetRotation(float* rot);
	void SetRotation(float x, float y, float z);
	void SetScale(float3 scale);
	float* GetEulerRot() const;
	float3 GetRotation() const;
	float3 GetPosition() const;

	AABB GetAABB() const;
	void SetAABB(AABB aabb);
	void Draw_AABB();
	void UpdateBoundingBoxes();
	
	//Debug Things...
	void SetGlobalTransform(float3 pos, Quat rot, float3 scale);

};

