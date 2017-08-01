#pragma once

#include "Globals.h"
#include <list>
#include "Engine/CoreModules/M_Window.h"
#include "Engine/CoreModules/M_Renderer.h"
#include "Engine/UI_Modules/M_UIManager.h"
#include "Engine/CoreModules/M_Input.h"
#include "Engine/CoreModules/M_FileSystem.h"
#include <GameConsole.h>
#include <SimpleLog.h>

class Module;
class M_FileSystem;

using namespace std::placeholders;

class Application : public IJsonSerializable
{
public:
	bool quit;
	/**
	 * \brief A map to store config path of all modules
	 * Key: module name
	 * Value: path
	 */
	std::map<std::string, std::string> configPath;
	M_Window* window = nullptr;
	M_Renderer* renderer = nullptr;
	M_UIManager* uiManager = nullptr;
	M_Input* input = nullptr;
	M_FileSystem* fs = nullptr;

	std::string organization;
	std::string appName;

	//Console
	std::istringstream* inputStream = nullptr;
	std::ostringstream* outputStream = nullptr;

	Virtuoso::SimpleLog log;
	Virtuoso::GameConsole console;

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

	void Serialize(Json::Value& root) override;
	void Deserialize(Json::Value& root) override;
	void LoadConfig();
	void SaveConfig();
private:
	void PrepareUpdate();
	void FinishUpdate();


};

extern Application* App;