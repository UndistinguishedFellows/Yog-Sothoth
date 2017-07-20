#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::GameObject(GameObject* parent)
{
	this->parent = parent;
}

GameObject::~GameObject()
{
	RemoveChildren();
}

