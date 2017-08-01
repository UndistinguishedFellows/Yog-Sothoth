#include "GameObject.h"
#include <stack>

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

bool GameObject::FindChild(GameObject* go)
{
	bool ret = false;
	std::stack<GameObject*> stack;
	stack.push(this);
	while (!stack.empty())
	{
		GameObject* top = stack.top();
		if (top == go)
		{
			ret = true;
			break;
		}
		stack.pop();
		for (int it = 0; it != top->children.size(); ++it)
		{
			stack.push(top->children[it]);
		}
	}

	return  ret;
}

GameObject* GameObject::FindChild(std::string goName)
{
	GameObject* ret = nullptr;
	std::stack<GameObject*> stack;
	stack.push(this);
	while (!stack.empty())
	{
		GameObject* top = stack.top();
		if (top->name == goName)
		{
			ret = top;
			break;
		}
		stack.pop();
		for (int it = 0; it != top->children.size(); ++it)
		{
			stack.push(top->children[it]);
		}
	}

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
