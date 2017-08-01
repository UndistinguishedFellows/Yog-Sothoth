#include "M_ObjectManager.h"



M_ObjectManager::M_ObjectManager(bool enabled): Module(enabled), deletionGameObject(nullptr)
{
	root = new GameObject();
	root->name = "/";
}


M_ObjectManager::~M_ObjectManager()
{
}

bool M_ObjectManager::Init()
{
	return true;
}

bool M_ObjectManager::Start()
{
	return true;
}
//Todo: Here game objects will be deleted when needed
update_status M_ObjectManager::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

//Todo: Game objects logic
update_status M_ObjectManager::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status M_ObjectManager::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool M_ObjectManager::CleanUp()
{
	return true;
}

GameObject* M_ObjectManager::CreateGameObject(GameObject* parent)
{
	GameObject* gm = new GameObject();
	if (parent != nullptr)
	{
		gm->parent = parent;
		parent->AddChild(gm);
	}

	return gm;
}

GameObject* M_ObjectManager::CreateGameObject(std::string parentName, std::string childName) const
{
	GameObject* parent = root->FindChild(parentName);
	GameObject* child = CreateGameObject(parent);
	child->name = childName;
	return child;
}

void M_ObjectManager::ConsoleCreateGameObject(std::string parentName, std::string childName) const
{
	GameObject* parent = root->FindChild(parentName);
	GameObject* child = CreateGameObject(parent);
	child->name = childName;

}

void M_ObjectManager::DeleteGameObject(GameObject* go)
{
	deletionGameObject->AddChild(go);
}

void M_ObjectManager::Serialize(Json::Value& root)
{
}

void M_ObjectManager::Deserialize(Json::Value& root)
{
}

void M_ObjectManager::LoadConfig()
{
}

void M_ObjectManager::SaveConfig()
{
}
