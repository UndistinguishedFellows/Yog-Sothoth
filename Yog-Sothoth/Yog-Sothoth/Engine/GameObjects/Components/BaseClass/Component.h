#pragma once
#include "../../Tools/Interfaces/IJsonSerializable.h"
class GameObject;
enum ComponentType
{
	CAMERA,
	TRANSFORM,
	MESH,
	TEXTURE,
	UNKNOWN
};

class Component : public IJsonSerializable
{
public:
	GameObject* parent = nullptr;
	ComponentType type = UNKNOWN;

	Component(GameObject* parent) { this->parent = parent; }
	virtual ~Component(){}

	void Serialize(Json::Value& root) {};
	void Deserialize(Json::Value& root) {};

};

