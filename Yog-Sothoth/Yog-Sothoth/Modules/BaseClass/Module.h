#pragma once
#include "../../Globals.h"
#include <string>
class Application;


class Module
{
public:
	std::string name;
private:
	bool enabled;

public:
	Module(bool enabled): enabled(enabled)
	{
	}

	virtual ~Module(){}

	bool IsEnabled() const
	{
		return enabled;
	}

	void Enable()
	{
		if (enabled == false)
		{
			enabled = true;
			Start();
		}
	}

	bool Disable()
	{
		bool ret = true;
		if (enabled == true)
		{
			enabled = false;
			ret = CleanUp();
		}
		return ret;
	}

	virtual bool Init()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}
	virtual update_status PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}
	virtual update_status Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	bool operator==(Module* module)
	{
		bool ret = false;
		if (module->name.compare(this->name) == 0)
			ret = true;
		return ret;
	}

};

