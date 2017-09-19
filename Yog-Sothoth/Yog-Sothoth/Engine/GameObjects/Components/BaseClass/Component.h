#pragma once
enum ComponentType
{
	CAMERA,
	TRANSFORM,
	MESH,
	TEXTURE,
	UNKNOWN
};

class Component
{
public:
	ComponentType type = UNKNOWN;
	Component(){}
	virtual ~Component(){}
};

