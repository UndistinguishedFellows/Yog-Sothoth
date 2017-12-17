#pragma once
#include <map>
#include "../../Tools/Containers/Shader.h"
#include "../../Tools/Interfaces/IJsonSerializable.h"
#include "BaseClass/Module.h"
#include "../Resources/Resource.h"

class M_ResourceManager : public Module, IJsonSerializable
{
public:
	bool reloadShaders = false;
	std::map<std::string, Shader*> shaders;
	std::map<UUID32, Resource*> resources;
	std::map<UUID32, Resource*> resourcesToDelete;

	M_ResourceManager(bool enable = true);
	~M_ResourceManager();

	bool Init() override;
	bool Start() override;
	update_status PreUpdate(float dt) override;
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp() override;

	Resource* LoadResource(UUID32 uuid, rType type);
	Resource* UnloadResource(UUID32 uuid);

	std::pair<std::string, Shader*> GetShader(std::string shaderName);

	virtual void Serialize(Json::Value& root);
	virtual void Deserialize(Json::Value& root);
	void LoadConfig();
	void SaveConfig();

};
