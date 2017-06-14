#pragma once

#include "Globals.h"
#include <list>
#include "Engine/CoreModules/M_Window.h"
#include "Engine/CoreModules/M_Renderer.h"
#include "Engine/UI_Modules/M_UIManager.h"
#include "Engine/CoreModules/M_Input.h"
#include "Engine/CoreModules/M_FileSystem.h"

class Module;
class M_FileSystem;

class Application
{
public:
	bool quit;
	M_Window* window = nullptr;
	M_Renderer* renderer = nullptr;
	M_UIManager* uiManager = nullptr;
	M_Input* input = nullptr;
	M_FileSystem* fs = nullptr;

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