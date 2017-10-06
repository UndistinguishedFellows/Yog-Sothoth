#include "GameObject.h"
#include <stack>
#include "Components/C_Camera.h"
#include "Components/C_Mesh.h"
#include "Components/C_Transform.h"
#include "Components/C_LIGHT.h"
#include <algorithm>

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
	for (auto component : components)
	{
		delete component;
	}
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
	for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end();)
	{
		if ((*it) == child)
		{
			it = children.erase(it);
		}
		else
			++it;
	}

}

void GameObject::RemoveChildren()
{
	for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end();)
	{
		if (children.size() > 0)
		{
			delete (*it);
			it = children.erase(it);
		}
		else
		{
			++it;
		}
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

void GameObject::LookAt(float3 pos)
{
	C_Camera* camera = (C_Camera*)FindComponent(C_CAMERA);
	if (camera != nullptr)
	{
		camera->LookAt(pos);

	}
}

void GameObject::SetPos(float3 pos)
{
	C_Transform* transform = (C_Transform*)FindComponent(C_TRANSFORM);
	if (transform != nullptr)
	{
		transform->SetPosition(pos);
	}	
}

AABB GameObject::UpdateBoundingBoxes()
{
	aabb.SetNegativeInfinity();
	std::vector<AABB> aabbVector;
	
	C_Mesh* mesh = (C_Mesh*)FindComponent(C_MESH);
	if (mesh != nullptr)
	{
		mesh->UpdateBoundingBoxes();
		aabbVector.push_back(mesh->GetAABB());
		for (auto child : children)
		{
			aabbVector.push_back(child->UpdateBoundingBoxes());
		}

		for (std::vector<AABB>::iterator iterator = aabbVector.begin(); iterator != aabbVector.end(); iterator++)
		{
			aabb.Enclose((*iterator));
		}

	}

	return aabb;
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
		if (type != GO_LIGHT && mesh->drawNormals)
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

AABB GameObject::GetAABB()
{
	UpdateBoundingBoxes();
	return aabb;
}

std::vector<GameObject*> GameObject::GetChildren()
{
	return children;
}

void GameObject::MoveChild(GameObject* child, GameObject* origin, GameObject* destiny)
{
	if (origin->FindChild(child))
	{
		origin->EraseChild(child);
		destiny->AddChild(child);
	}
}

void GameObject::Draw_AABB()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	glLineWidth(1.f);
	glColor4f(0.f, 1.f, 0.f, 1.f);

	float3 vertices[8];
	aabb.GetCornerPoints(vertices);

	//glColor4f(color.r, color.g, color.b, color.a);

	glBegin(GL_QUADS);

	glVertex3fv((GLfloat*)&vertices[1]);
	glVertex3fv((GLfloat*)&vertices[5]);
	glVertex3fv((GLfloat*)&vertices[7]);
	glVertex3fv((GLfloat*)&vertices[3]);

	glVertex3fv((GLfloat*)&vertices[4]);
	glVertex3fv((GLfloat*)&vertices[0]);
	glVertex3fv((GLfloat*)&vertices[2]);
	glVertex3fv((GLfloat*)&vertices[6]);

	glVertex3fv((GLfloat*)&vertices[5]);
	glVertex3fv((GLfloat*)&vertices[4]);
	glVertex3fv((GLfloat*)&vertices[6]);
	glVertex3fv((GLfloat*)&vertices[7]);

	glVertex3fv((GLfloat*)&vertices[0]);
	glVertex3fv((GLfloat*)&vertices[1]);
	glVertex3fv((GLfloat*)&vertices[3]);
	glVertex3fv((GLfloat*)&vertices[2]);

	glVertex3fv((GLfloat*)&vertices[3]);
	glVertex3fv((GLfloat*)&vertices[7]);
	glVertex3fv((GLfloat*)&vertices[6]);
	glVertex3fv((GLfloat*)&vertices[2]);

	glVertex3fv((GLfloat*)&vertices[0]);
	glVertex3fv((GLfloat*)&vertices[4]);
	glVertex3fv((GLfloat*)&vertices[5]);
	glVertex3fv((GLfloat*)&vertices[1]);

	glEnd();

	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_LIGHTING);
	//glLineWidth(1.f);

}
