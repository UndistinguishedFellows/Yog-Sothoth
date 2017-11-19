#include "GameObject.h"
#include <stack>
#include "Components/C_Camera.h"
#include "Components/C_Mesh.h"
#include "Components/C_Transform.h"
#include "Components/C_LIGHT.h"
#include <algorithm>
#include "../../Tools/Primitive.h"
#include "../../Application.h"
#include "../../Tools/Static/JsonSerializer.h"

GameObject::GameObject()
{
}

GameObject::GameObject(GameObject* parent)
{
	this->parent = parent;
	parent->children.push_back(this);
	shader = App->resourceManager->shaders["objectShader"];
}

GameObject::~GameObject()
{
	for (auto component : components)
	{
		RELEASE(component);
	}
	RemoveChildren();
}

void GameObject::DoPostUpdate()
{
}

void GameObject::RefreshTransform()
{
	bool ret = false;
	std::stack<GameObject*> stack;
	stack.push(this);
	while (!stack.empty())
	{
		GameObject* top = stack.top();
		if (top->Transform != nullptr)
		{
			top->Transform->RefreshTransform();
		}		

		stack.pop();
		for (int it = 0; it != top->children.size(); ++it)
		{
			stack.push(top->children[it]);
		}
	}
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
			RELEASE((*it));
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
			RELEASE((*it));
			it = children.erase(it);
			int i = children.size();
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

	}
	for (auto child : children)
	{
		aabbVector.push_back(child->UpdateBoundingBoxes());
	}

	for (std::vector<AABB>::iterator iterator = aabbVector.begin(); iterator != aabbVector.end(); iterator++)
	{
		aabb.Enclose((*iterator));
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
			camera = static_cast<C_Camera*>(ret);
		}
		break;
		case C_MESH:
		{
			ret = new C_Mesh(this);
			ret->type = C_MESH;
			this->components.push_back(ret);
			mesh = static_cast<C_Mesh*>(ret);
		}
		break;
		case C_TRANSFORM:
		{
			ret = new C_Transform(this);
			ret->type = C_TRANSFORM;
			this->components.push_back(ret);
			transform = static_cast<C_Transform*>(ret);
		}
		break;
		case C_LIGHT:
		{
			ret = new C_Light(this);
			ret->type = C_LIGHT;
			ret->ownerParent->type = GO_LIGHT;
			this->components.push_back(ret);
			light = static_cast<C_Light*>(ret);
		}
		break;
		case C_MATERIAL:
		{
			ret = new C_Material(this);
			ret->type = C_MATERIAL;
			this->components.push_back(ret);
			material = static_cast<C_Material*>(ret);
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
void GameObject::SendToDraw()
{
	App->renderer->drawVector.push_back(this);
}

void GameObject::RemoveFromDraw()
{
	std::vector<GameObject*>::iterator it = std::find(App->renderer->drawVector.begin(), App->renderer->drawVector.end(), this);
	App->renderer->drawVector.erase(it);
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

void GameObject::DrawAABB(Shader shader, C_Camera* camera)
{
	//UpdateBoundingBoxes();
	float4x4 view = camera->frustum.ViewMatrix();
	float3 size = aabb.Size();
//	OBB obb = aabb;	
//	obb.Transform(this->Transform->globalTransform);
//	aabb.SetFrom(obb);
	//aabb.Enclose(obb);
	Quat rot;
	float3 pos;
	float3 scale;
	this->Transform->globalTransform.Decompose(pos, rot, scale);
	float4x4 model = float4x4::identity;

	model = model.Scale(size).ToFloat4x4();
	model.SetTranslatePart(aabb.CenterPoint());

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	shader.Use();
	glBindVertexArray(App->objManager->primitives->pCube.VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, App->objManager->primitives->pCube.indices.idIndices);
	shader.setMat4("view", &view.Transposed());
	shader.setMat4("projection", &App->objManager->activeCamera->Camera->frustum.ProjectionMatrix().Transposed());
	shader.setMat4("model", &model.Transposed());
	shader.setVec4("objectColor", &float4(1.f, 0.f, 0.5f, 1.0f));
	glLineWidth(2.0f);
	//glPointSize(10.0f);
	//glDrawElements(GL_POINTS, primi.pCube.indices.numIndices, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, App->objManager->primitives->pCube.indices.numIndices, GL_UNSIGNED_INT, 0);
	glLineWidth(1.0f);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

AABB GameObject::GetAABB()
{
	//UpdateBoundingBoxes();
	return aabb;
}

void GameObject::Serialize(Json::Value& root)
{
	std::stack<GameObject*> stack;
	stack.push(this);

	while(!stack.empty())
	{
		GameObject* top = stack.top();

		LCG random;
		do	top->uuid = random.Int(); while (top->uuid == 0);
		
		Json::Value gameObject;
		gameObject["type"] = top->type;
		gameObject["name"] = top->name;
		if (top->parent != nullptr && top != this)
			gameObject["parent_uuid"] = top->parent->uuid;
		else
			gameObject["parent_uuid"] = 0;
		
		gameObject["uuid"] = top->uuid;
		gameObject["active"] = top->active;

		for (std::vector<Component*>::iterator it = top->components.begin(); it != top->components.end(); ++it)
		{
			Json::Value jComp;
			(*it)->Serialize(jComp);
			gameObject["components"].append(jComp);
		}

		stack.pop();
		for (int it = 0; it != top->children.size(); ++it)
		{
			stack.push(top->children[it]);
		}
		root["gameObjects"].append(gameObject);
		
	}

	
}

void GameObject::Deserialize(Json::Value& root)
{
	Json::Value j_go = root.get("gameObjects", 0);
	std::vector<GameObject*> unOrderedGO;
	for (int i = 0; i != j_go.size(); i++)
	{
		GameObject* go = new GameObject();
		go->name = j_go[i].get("name", "no_name").asString();
		go->active = j_go[i].get("active", false).asBool();
		go->parent_uuid = j_go[i].get("parent_uuid", 0).asInt64();
		go->type = (GameObjectType)j_go[i].get("type", 0).asInt();
		go->uuid = j_go[i].get("uuid", 0).asInt64();

		Json::Value j_comp = j_go[i]["components"];
		for (int j = 0; j != j_comp.size(); j++)
		{
			switch (j_comp[j].get("type", 0).asInt())
			{
				case 0:
				default:
					break;
				case C_CAMERA:
				{
					C_Camera* camera = (C_Camera*)go->CreateComponent(C_CAMERA);
					camera->Deserialize(j_comp[j]);
					break;
				}
				case C_TRANSFORM:
				{
					C_Transform* transform = (C_Transform*)go->CreateComponent(C_TRANSFORM);
					transform->Deserialize(j_comp[j]);
					break;
				}
				case C_MESH:
				{
					C_Mesh* mesh = (C_Mesh*)go->CreateComponent(C_MESH);
					mesh->Deserialize(j_comp[j]);
					break;
				}
				case C_MATERIAL:
				{
					C_Material* material = (C_Material*)go->CreateComponent(C_MATERIAL);
					material->Deserialize(j_comp[j]);
					break;
				}
				case C_LIGHT:
				{
					C_Light* light = (C_Light*)go->CreateComponent(C_LIGHT);
					light->Deserialize(j_comp[j]);
					break;
				}
			}
		}
		unOrderedGO.push_back(go);
	}
	for (auto go : unOrderedGO)
	{
		if (go->parent_uuid == 0)
		{
			this->AddChild(go);
		}
		else
		{
			for (auto s : unOrderedGO)
			{
				if (s->uuid == go->parent_uuid)
				{
					s->AddChild(go);
				}
			}
		}
		if (go->Mesh && go->Material)
		{
			go->Mesh->associatedMaterial = go->Material;
		}
	}
	Transform->RefreshTransform();

	
	UpdateBoundingBoxes();
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

void GameObject::Save(std::string fileName)
{
	std::string output;
	JsonSerializer::Serialize(this, output, fileName);
}
