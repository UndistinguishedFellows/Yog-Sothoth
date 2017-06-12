#pragma once

#include "BaseClass/Module.h"

class M_Camera3D : Module
{
public:
	M_Camera3D(bool enabled = true);
	~M_Camera3D();

	bool Init() override;
	bool Start() override;
	update_status PreUpdate(float dt) override;
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp() override;


	
};
