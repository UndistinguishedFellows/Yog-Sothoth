#include "GameObject.h"
#include <stack>
#include "Components/C_Camera.h"
#include "Components/C_Mesh.h"
#include "Components/C_Transform.h"
#include "Components/C_LIGHT.h"

GameObject::GameObject()
{
}

GameObject::GameObject(GameObject* parent)
{
	this->parent = parent;
	parent->children.push_back(this);
}

GameObject::~GameObject()
{
	RemoveChildren();
}

void GameObject::AddChild(GameObject* child)
{
	children.push_back(child);
	child->parent = this;
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

Component* GameObject::CreateComponent(ComponentType type)
{
	Component* ret = nullptr;
	switch (type)
	{
		case C_CAMERA:
		{
			ret = new C_Camera(this);
			ret->type = C_CAMERA;
			this->components.push_back(ret);
		}
		break;
		case C_MESH:
		{
			ret = new C_Mesh(this);
			ret->type = C_MESH;
			this->components.push_back(ret);
		}
		break;
		case C_TRANSFORM:
		{
			ret = new C_Transform(this);
			ret->type = C_TRANSFORM;
			this->components.push_back(ret);
		}
		break;
		case C_LIGHT:
		{
			ret = new C_Light(this);
			ret->type = C_TRANSFORM;
			this->components.push_back(ret);
		}
		break;
	}

	return ret;
}

Component* GameObject::FindComponent(ComponentType type)
{
	for (auto component : components)
	{
		if (component->type == type)
		{
			return component;
		}
	}
	return nullptr;
}

void GameObject::Draw(Shader shader, C_Camera* camera)
{
	C_Mesh* mesh = (C_Mesh*)FindComponent(C_MESH);
	if (mesh != nullptr)
	{
		if (type != GO_LIGHT)
		{
			mesh->Draw(shader, camera);
		}
	}
	

	for (auto child : children)
	{
		child->Draw(shader, camera);
		
	}
}

void GameObject::DrawNormals(Shader shader, C_Camera* camera)
{
	C_Mesh* mesh = (C_Mesh*)FindComponent(C_MESH);
	if (mesh != nullptr)
	{
		if (type != GO_LIGHT)
		{
			mesh->DrawNormals(shader, camera);
		}
	}


	for (auto child : children)
	{
		child->DrawNormals(shader, camera);

	}

}

void GameObject::DrawLight(Shader shader, C_Camera* camera)
{
	C_Mesh* mesh = (C_Mesh*)FindComponent(C_MESH);
	if (mesh != nullptr)
	{
		mesh->Draw(shader, camera);
	}
}
