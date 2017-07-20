#pragma once
#include <vector>

class GameObject
{
public:
	std::vector<GameObject*> children;
	GameObject* parent = nullptr;

	bool active = true;

public:
	GameObject();
	GameObject(GameObject* parent);
	~GameObject();

};
