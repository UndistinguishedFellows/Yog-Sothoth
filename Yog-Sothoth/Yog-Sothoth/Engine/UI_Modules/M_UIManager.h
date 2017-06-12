#pragma once
#include "../../Globals.h"
#include "../CoreModules/BaseClass/Module.h"

class M_UIManager : public Module
{
public:
	M_UIManager(bool enabled = true);
	~M_UIManager();

	bool Init() override;
	bool Start() override;
	update_status PreUpdate(float dt) override;
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp() override;


	
};
