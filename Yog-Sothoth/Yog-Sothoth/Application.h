#pragma once

#include "Globals.h"
#include <list>
#include "Modules/M_Window.h"
#include "Modules/M_Renderer.h"

class Module;

class Application
{
public:
	bool quit;
	M_Window* window = nullptr;
	M_Renderer* renderer = nullptr;

private:
	std::list<Module*> list_modules;

public:
	Application();
	~Application();

	bool Init();
	update_status Update(float dt);
	bool CleanUp();

	bool OpenBrowser(const char* link);
	void Log(char* str);

	char* getOrganization();
	char* getTitle();

	void addModule(Module* module);
	void removeModule(Module* module);

private:
	void PrepareUpdate();
	void FinishUpdate();


};

extern Application* App;