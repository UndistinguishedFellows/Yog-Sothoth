#pragma once
#include "../CoreModules/BaseClass/Module.h"

class M_ObjectManager :
	public Module
{
public:
	M_ObjectManager(bool enabled = true);
	~M_ObjectManager();

	bool Init() override;
	bool Start() override;
	update_status PreUpdate(float dt) override;
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp() override;

	void Serialize(Json::Value& root) override;
	void Deserialize(Json::Value& root) override;
	void LoadConfig() override;
	void SaveConfig() override;




};

