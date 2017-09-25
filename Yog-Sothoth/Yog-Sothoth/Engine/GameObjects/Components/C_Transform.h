#pragma once
#include "BaseClass/Component.h"
#include "../../../MathGeoLib/Math/float4x4.h"
#include "../../../MathGeoLib/Geometry/AABB.h"
#include "../../../Tools/Containers/Color.h"

class C_Transform :
	public Component
{
public:
	// Father dependant transform
	float4x4 localTransform = float4x4::identity;
	// C_Transform dependant from the origin
	float4x4 globalTransform = float4x4::identity;

	AABB aabb;
	Color aabb_color;
	
	C_Transform(GameObject* parent);
	~C_Transform();

	const float4x4 getLocalTransform();
	const float4x4 getGlobalTransform();

	void refreshTransform(float4x4 mat);

	void setPosition(float3 position);
	void setRotation(Quat rotation);
	void setRotation(float* rot);
	void setRotation(float x, float y, float z);
	void setScale(float3 scale);
	float* getEulerRot() const;
	float3 getRotation() const;

	AABB getAABB() const;
	void setAABB(AABB aabb);
	void draw_AABB();
	void updateBoundingBoxes();
	
	//Debug Things...
	void setGlobalTransform(float3 pos, Quat rot, float3 scale);

};

