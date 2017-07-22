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

void GameObject::AddRelationship(GameObject** reference)
{
	if(reference != nullptr)
		relationship.push_back(reference);
}

void GameObject::EraseRelationship(GameObject** reference)
{
	if (reference != nullptr)
		for (std::vector<GameObject**>::iterator it = relationship.begin();
				it != relationship.end(); ++it)
		{
			if (reference == (*it))
			{
				(*reference) = nullptr;
				relationship.erase(it);
			}
		}
}
