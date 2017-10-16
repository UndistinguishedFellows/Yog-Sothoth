#pragma once
#include <map>
#include "../../Tools/Containers/Shader.h"
#include "../../Tools/Interfaces/IJsonSerializable.h"
#include "BaseClass/Module.h"

class M_ResourceManager : public Module, IJsonSerializable
{
public:
	std::map<std::string, Shader*> shaders;

	M_ResourceManager(bool enable = true);
	~M_ResourceManager();

	bool Init() override;
	bool Start() override;
	update_status PreUpdate(float dt) override;
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp() override;

	virtual void Serialize(Json::Value& root);
	virtual void Deserialize(Json::Value& root);
	void LoadConfig();
	void SaveConfig();

};
