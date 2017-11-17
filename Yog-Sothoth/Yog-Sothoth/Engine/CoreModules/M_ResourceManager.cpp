﻿#include "M_ResourceManager.h"
#include "../../Tools/Static/JsonSerializer.h"
#include "../Resources/R_Mesh.h"

M_ResourceManager::M_ResourceManager(bool enable) : Module(enable)
{
	name = "resource manager";
}

M_ResourceManager::~M_ResourceManager()
{
}

bool M_ResourceManager::Init()
{
	return true;
}

bool M_ResourceManager::Start()
{
	LoadConfig();
	return true;
}

update_status M_ResourceManager::PreUpdate(float dt)
{
	for (std::map<UUID32, Resource*>::iterator it = resourcesToDelete.begin(); 
		 it != resourcesToDelete.end(); ++it)
	{
		resources.erase((*it).first);
		Resource* res = (*it).second;
		resourcesToDelete.erase(it);
		RELEASE(res);
	}
	return UPDATE_CONTINUE;
}

update_status M_ResourceManager::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status M_ResourceManager::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool M_ResourceManager::CleanUp()
{
	return true;
}

void M_ResourceManager::Serialize(Json::Value& root)
{
}

void M_ResourceManager::Deserialize(Json::Value& root)
{
	Json::Value::Members members = root.getMemberNames();

	for (int it = 0; it < members.size(); ++it)
	{
		Json::Value shdr = root.get(members[it], "noShader");
		std::string shaderName= members[it];
		std::string vertex;
		std::string fragment;
		std::string geometry;

		vertex = shdr.get("vertex", "noShader").asString();
		fragment = shdr.get("fragment", "noShader").asString();
		geometry = shdr.get("geometry", "noShader").asString();

		Shader* shader;
		if (strcmp(geometry.c_str(), "noShader") == 0)
		{
			shader = new Shader(vertex.c_str(), fragment.c_str());
		}
		else
		{
			shader = new Shader(vertex.c_str(), fragment.c_str(), geometry.c_str());
		}

		shaders.insert(std::pair<std::string, Shader*>(shaderName, shader));
	}
}
void M_ResourceManager::LoadConfig()
{
	JsonSerializer::DeserializeFormPath(this, "data/shaders/shadersConfig.json");
}

void M_ResourceManager::SaveConfig()
{
	std::string output;
	JsonSerializer::Serialize(this, output, "data/shaders/shadersConfig.json");
	SDL_Log("%s", output);
}

Resource* M_ResourceManager::LoadResource(UUID32 uuid, rType type)
{
	Resource* ret = nullptr;
	std::map<UUID32, Resource*>::iterator it = resources.find(uuid);
	if (it == resources.end())
	{
		switch (type)
		{
		case R_MESH: 
		{
			R_Mesh* res = new R_Mesh();
			std::string name = "data/library/";
			name.append(std::to_string(uuid));
			res->LoadMeshFile(name.c_str());
			ret = res;
			break;
		}
		case R_MATERIAL: 
			break;
		case R_UNKNOWN:
		default: 
			break;
		}
	}
	else
	{
		switch (type)
		{
			case R_MESH:
				ret = (*it).second;
				break;
			case R_MATERIAL:
				break;
			case R_UNKNOWN:
			default:
				break;
		}

	}
	if (ret != nullptr)
	{
		ret->used++;
	}
}

Resource* M_ResourceManager::UnloadResource(UUID32 uuid)
{
	Resource* ret = nullptr;
	std::map<UUID32, Resource*>::iterator it = resources.find(uuid);
	if (it != resources.end())
	{
		ret = (*it).second;
		
		if (--ret->used == 0)
		{
			resourcesToDelete.insert((*it));
		}
	}


	return ret;
}
