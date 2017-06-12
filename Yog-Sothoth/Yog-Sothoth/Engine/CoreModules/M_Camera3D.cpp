	
#include "M_Camera3D.h"

M_Camera3D::M_Camera3D(bool enabled) : Module(enabled)
{
	name.assign("camera3D");
}

M_Camera3D::~M_Camera3D()
{
}

bool M_Camera3D::Init()
{
	return true;
}

bool M_Camera3D::Start()
{
	return true;
}

update_status M_Camera3D::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status M_Camera3D::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status M_Camera3D::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool M_Camera3D::CleanUp()
{
	return true;
}
