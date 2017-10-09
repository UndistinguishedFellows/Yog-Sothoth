#pragma once
#include "../../Tools/Interfaces/IJsonSerializable.h"
class GameObject;
enum ComponentType
{
	C_CAMERA,
	C_TRANSFORM,
	C_MESH,
	C_MATERIAL,
	C_LIGHT,
	C_UNKNOWN
};

class Component : public IJsonSerializable
{
public:
	GameObject* parent = nullptr;
	ComponentType type = C_UNKNOWN;

	Component(GameObject* parent) { this->parent = parent; }
	virtual ~Component(){}

	void Serialize(Json::Value& root) {};
	void Deserialize(Json::Value& root) {};

};

