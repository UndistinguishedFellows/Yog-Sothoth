#include "M_ResourceManager.h"
#include "../../Tools/Static/JsonSerializer.h"
#include "../Resources/R_Mesh.h"
#include "../Resources/R_Material.h"
#include <ilut.h>

M_ResourceManager::M_ResourceManager(bool enable) : Module(enable)
{
	name = "resource manager";
}

M_ResourceManager::~M_ResourceManager()
{
}

bool M_ResourceManager::Init()
{
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
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
		 it != resourcesToDelete.end();)
	{
		resources.erase((*it).first);
		Resource* res = (*it).second;
		it = resourcesToDelete.erase(it);
		RELEASE(res);
	}
	if (shaders.size() > 2)
	{
		printf(">2");
	}
	if (reloadShaders)
	{		
		for (auto item : shaders)
		{
			item.second->LoadShader();
		}
		reloadShaders = false;
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
			name.append(".mesh");
			res->LoadMeshFile(name.c_str());
			ret = res;
			std::pair<UUID32, Resource*> pair;
			pair.first = uuid;
			pair.second = res;
			resources.insert(pair);
			break;
		}
		case R_MATERIAL: 
		{
			R_Material* res = new R_Material();
			std::string name = "data/library/";
			name.append(std::to_string(uuid));
			name.append(".dds");
			res->LoadMaterialFile(name.c_str());
			ret = res;
			std::pair<UUID32, Resource*> pair;
			pair.first = uuid;
			pair.second = res;
			resources.insert(pair);

			break;
		}
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
				ret = (*it).second;
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

	return ret;
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

std::pair<std::string, Shader*> M_ResourceManager::GetShader(std::string shaderName)
{
	std::map<std::string, Shader*>::iterator iterator = shaders.find(shaderName);

	return *iterator;
}
