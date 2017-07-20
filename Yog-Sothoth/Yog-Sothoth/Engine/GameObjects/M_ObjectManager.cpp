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
