#include "C_Transform.h"
#include "../GameObject.h"
#include "C_Mesh.h"


C_Transform::C_Transform(GameObject* parent) : Component(parent)
{
	type = C_TRANSFORM;
}


C_Transform::~C_Transform()
{
}

const float4x4 C_Transform::GetLocalTransform() const
{
	return localTransform;
}

const float4x4 C_Transform::GetGlobalTransform() const
{
	return globalTransform;
}

void C_Transform::RefreshTransform()
{
	if (ownerParent != nullptr && ownerParent->parent->Transform != nullptr)
	{
		globalTransform = ownerParent->parent->Transform->globalTransform * localTransform;
	}

	for (std::vector<GameObject*>::iterator iterator = ownerParent->children.begin();
		 iterator != ownerParent->children.end(); ++iterator)
	{
		(*iterator)->Transform->RefreshTransform();
	}
	
}

void C_Transform::SetPosition(float3 position)
{
//	float3 pos;
//	float3 sca;
//	Quat rot;
//	localTransform.Decompose(pos, rot, sca);
//
//	localTransform = float4x4::FromTRS(position, rot, sca);
//
	localTransform.SetTranslatePart(position);
	//localTransform = localTransform.Translate(position);

	RefreshTransform();

	ownerParent->Transform->UpdateBoundingBoxes();

}

void C_Transform::SetRotation(Quat rotation)
{
	float3 pos;
	float3 sca;
	Quat rot;
	localTransform.Decompose(pos, rot, sca);

	localTransform = float4x4::FromTRS(pos, rotation, sca);
	RefreshTransform();

	if (ownerParent->Mesh != nullptr)
	{
		ownerParent->Mesh->UpdateBoundingBoxes();
	}


}

void C_Transform::SetRotation(float* rot)
{
	float3 r;
	r.Set(rot);

	while (r.x < 0)
		r.x += 360;
	while (r.y < 0)
		r.y += 360;
	while (r.z < 0)
		r.z += 360;

	while (r.x >= 360)
		r.x -= 360;
	while (r.y >= 360)
		r.y -= 360;
	while (r.z >= 360)
		r.z -= 360;

	r.x = DegToRad(r.x);
	r.y = DegToRad(r.y);
	r.z = DegToRad(r.z);

	SetRotation(Quat::FromEulerXYZ(r.x, r.y, r.z));

}

void C_Transform::SetRotation(float x, float y, float z)
{
	while (x < 0)
		x += 360;
	while (y < 0)
		y += 360;
	while (z < 0)
		z += 360;

	while (x >= 360)
		x -= 360;
	while (y >= 360)
		y -= 360;
	while (z >= 360)
		z -= 360;

	x = DegToRad(x);
	y = DegToRad(y);
	z = DegToRad(z);

	SetRotation(Quat::FromEulerXYZ(x, y, z));

}

void C_Transform::SetScale(float3 scale)
{
	float3 pos;
	float3 sca;
	Quat rot;
	localTransform.Decompose(pos, rot, sca);

	localTransform = float4x4::FromTRS(pos, rot, scale);
	RefreshTransform();

	ownerParent->Mesh->UpdateBoundingBoxes();

}

float* C_Transform::GetEulerRot() const
{
	float3 pos;
	float3 sca;
	Quat rot;
	localTransform.Decompose(pos, rot, sca);

	float3 rotationEuler = rot.ToEulerXYZ();

	rotationEuler.x = DegToRad(rotationEuler.x);
	rotationEuler.y = DegToRad(rotationEuler.y);
	rotationEuler.z = DegToRad(rotationEuler.z);

	while (rotationEuler.x < 0)
		rotationEuler.x += 360;
	while (rotationEuler.y < 0)
		rotationEuler.y += 360;
	while (rotationEuler.z < 0)
		rotationEuler.z += 360;

	while (rotationEuler.x >= 360)
		rotationEuler.x -= 360;
	while (rotationEuler.y >= 360)
		rotationEuler.y -= 360;
	while (rotationEuler.z >= 360)
		rotationEuler.z -= 360;

	return (float*)&rotationEuler;

}

float3 C_Transform::GetRotation() const
{
	float3 pos;
	float3 sca;
	Quat rot;
	localTransform.Decompose(pos, rot, sca);

	float3 ret = rot.ToEulerXYZ();

	ret.x = RadToDeg(ret.x);
	ret.y = RadToDeg(ret.y);
	ret.z = RadToDeg(ret.z);

	while (ret.x < 0)
		ret.x += 360;
	while (ret.y < 0)
		ret.y += 360;
	while (ret.z < 0)
		ret.z += 360;

	while (ret.x >= 360)
		ret.x -= 360;
	while (ret.y >= 360)
		ret.y -= 360;
	while (ret.z >= 360)
		ret.z -= 360;

	return ret;

}

float3 C_Transform::GetPosition() const
{
	float3 pos;
	float3 sca;
	Quat rot;
	localTransform.Decompose(pos, rot, sca);
	return pos;
}

void C_Transform::UpdateBoundingBoxes()
{
}

void C_Transform::Serialize(Json::Value& root)
{
	float3 position;
	float3 scale;
	Quat rotation;

	localTransform.Decompose(position, rotation, scale);


	root["position"].append(position.x);
	root["position"].append(position.y);
	root["position"].append(position.z);

	root["rotation"].append(rotation.x);
	root["rotation"].append(rotation.y);
	root["rotation"].append(rotation.z);
	root["rotation"].append(rotation.w);

	root["scale"].append(scale.x);
	root["scale"].append(scale.y);
	root["scale"].append(scale.z);

	root["type"] = type;
}

void C_Transform::Deserialize(Json::Value& root)
{
	float3 pos;
	float3 scale;
	Quat rot;


	//Position
	Json::Value jpos = root["position"];
	for (int i = 0; i != jpos.size(); i++)
		*(pos.ptr() + i) = jpos[i].asFloat();

	//Scale
	Json::Value jscale = root["scale"];
	for (int i = 0; i != jscale.size(); i++)
		*(scale.ptr() + i) = jscale[i].asFloat();

	//Rotation
	Json::Value jRot = root["rotation"];
	for (int i = 0; i != jRot.size(); i++)
		*(rot.ptr() + i) = jRot[i].asFloat();

	localTransform = float4x4::FromTRS(pos, rot, scale);

}