#pragma once

#include "BaseClass/Module.h"

class M_Renderer : public Module
{
public:
	SDL_GLContext context;

public:
	M_Renderer(bool enabled = true);
	~M_Renderer();

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
