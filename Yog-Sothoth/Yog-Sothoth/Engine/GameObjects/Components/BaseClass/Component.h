#pragma once
#include "../../Tools/Interfaces/IJsonSerializable.h"
class GameObject;
enum ComponentType
{
	C_UNKNOWN,
	C_CAMERA,
	C_TRANSFORM,
	C_MESH,
	C_MATERIAL,
	C_LIGHT	
};

class Component : public IJsonSerializable
{
public:
	GameObject* ownerParent = nullptr;
	ComponentType type = C_UNKNOWN;

	Component(GameObject* parent) { this->ownerParent = parent; }
	virtual ~Component(){}

	void Serialize(Json::Value& root) {};
	void Deserialize(Json::Value& root) {};

};

