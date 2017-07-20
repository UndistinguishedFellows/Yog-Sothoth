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

void GameObject::AddChild(GameObject* child)
{
	children.push_back(child);
}

void GameObject::RemoveChild(GameObject* child)
{
	for (std::vector<GameObject*>::iterator it = children.begin(); 
		 it != children.end(); ++it)
	{
		if ((*it) == child)
		{
			delete (*it);
			children.erase(it);
		}
	}
}
void GameObject::EraseChild(GameObject* child)
{
	for (std::vector<GameObject*>::iterator it = children.begin();
		 it != children.end(); ++it)
	{
		if ((*it) == child)
		{
			children.erase(it);
		}
	}

}

void GameObject::RemoveChildren()
{
	for (std::vector<GameObject*>::iterator it = children.begin();
		 it != children.end(); ++it)
	{
		(*it)->RemoveChildren();
		delete (*it);
		children.erase(it);
	}

}

int GameObject::FindChild(GameObject* go)
{
	int ret = -1;

	return  ret;
}
