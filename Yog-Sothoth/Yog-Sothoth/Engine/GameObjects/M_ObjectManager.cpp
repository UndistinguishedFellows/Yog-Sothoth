#include "M_ObjectManager.h"



M_ObjectManager::M_ObjectManager(bool enabled): Module(enabled)
{
}


M_ObjectManager::~M_ObjectManager()
{
}

bool M_ObjectManager::Init()
{
	return false;
}

bool M_ObjectManager::Start()
{
	return false;
}
//Todo: Here game objects will be deleted when needed
update_status M_ObjectManager::PreUpdate(float dt)
{
	return {};
}

//Todo: Game objects logic
update_status M_ObjectManager::Update(float dt)
{
	return {};
}

update_status M_ObjectManager::PostUpdate(float dt)
{
	return {};
}

bool M_ObjectManager::CleanUp()
{
	return false;
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

void M_ObjectManager::DeleteGameObject(GameObject* go)
{
	deletionVector.push_back(go);
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
